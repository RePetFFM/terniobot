#include <AFMotor.h>
#include <Wire.h>



#define DEFAULT_SERIAL_DATA_LENGTH

// define data types read
#define PING 200
#define REQUEST_STATUS 201
#define REQUEST_MOTORTARGET 202
#define PHI 211
#define VECTOR_X 212
#define VECTOR_Y 213
#define VECTOR_VELOCITY 214

#define TARGET_A 221
#define TARGET_B 222
#define TARGET_C 223

// define data types write
#define PONG 200
#define BOT_READY 201


// serial update const
#define UPDATE_PHI 1
#define UPDATE_VECTOR 2




AF_DCMotor motorA(1);
AF_DCMotor motorB(2);
AF_DCMotor motorC(3);



// interrupt variables

// omni wheel encoder
volatile int encA = 0;
volatile int encB = 0;
volatile int encC = 0;

volatile byte encAState = B00000000;
volatile byte encBState = B00000000;
volatile byte encCState = B00000000;

volatile float encAVelocity = 0.0;
volatile float encBVelocity = 0.0;
volatile float encCVelocity = 0.0;

float encArad = 0.0;
float encBrad = 0.0;
float encCrad = 0.0;

volatile long encALastT = 0;
volatile long encBLastT = 0;
volatile long encCLastT = 0;


int sequenceCounter = 0;


// basic values and constants
// omnidirectional wheel diameter 40mm, encoder rev ratio is 1.84714359238291
// encoder slits 35 LOW & HIGH Count Total of 129 impulse per rev


// constant math values
float pi = 3.14159;

float Ka = 0.0;
float Kb = 0.0;
float Kc = 0.0;

float piR = 0.0; // pi * omniwheel radius

// basic BOTi values
float deltaPosX = 0.0;
float deltaPosY = 0.0;
float currentAbsPosX = 0.0;
float currentAbsPosY = 0.0;

float V_x_vector = 0.0;
float V_y_vector = 0.0;

float cosPhi = 0.0;
float sinPhi = 0.0;

float yaw_rate = pi*0.0; // rate in rdian (A)

float phi = pi*0.0;

int wheelCenterDistance = 105;


volatile float vectorX = 0.0;
volatile float vectorY = 0.0;
float vectorVelocity = 0.0;

int arEncA = 0;
int arEncB = 0;
int arEncC = 0;

float encRevALast = 0.0;
float encRevBLast = 0.0;
float encRevCLast = 0.0;


int currentMSpeedA = 0.0;
int currentMSpeedB = 0.0;
int currentMSpeedC = 0.0;


float revBase = pi*2.0*1000000.0;
long deltaLastReading = 0;


float targetArev = 0.0;
float targetBrev = 0.0;
float targetCrev = 0.0;

int maxMotorRev = 250;
int minMotorRev = 50;
float maxRev = 60.0;

float debugIntegerA = 0.0;
float debugIntegerB = 0.0;
float debugIntegerC = 0.0;


float current_Vx = 0.0;
float current_Vy = 0.0;
float current_omegaMini = 0.0;

byte tm_execute_target_motion = 0; // 1 = execute, 0 = on hold
float tm_target_x = 0.0;
float tm_target_y = 0.0;
float tm_end_zone = 100.0;

int deltaXY = 0;

void setup() {

  // setup encoder analog input pin
  // activate pull up resistors
  pinMode(A1, INPUT);
  digitalWrite(A2,HIGH);

  pinMode(A3, INPUT);
  digitalWrite(A3,HIGH);

  pinMode(A2, INPUT);
  digitalWrite(A4,HIGH);

  // setup interrupt for encoder
  
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 64; // 32, 31250;            // compare match register 16MHz/256/2Hz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  // TCCR1B |= (1 << CS11);    // 64 prescaler 
  // TCCR1B |= (1 << CS10);    // 64 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts
  
  Serial.begin(19200);
  // pre calc constants
  Ka = (2.0*3.14159*20.0)/(3.0*102.87);
  Kb = (3.14159*20)/(3.0);
  Kc = Kb*sqrt(3.0);

  piR = pi*20.0; // pi * omniwheel radius

  
  // turn on motor
  motorA.setSpeed(0);
  motorB.setSpeed(0);
  motorC.setSpeed(0);
 
  motorA.run(RELEASE);
  motorB.run(RELEASE);
  motorC.run(RELEASE);

  

  // calculate zero setting
  calc_Vel_Vector(0.0,0.0,0.0,0.0,0.0); // float V, float x0, float x1, float y0, float y1
  cosPhi = cos(phi);
  sinPhi = sin(phi);
  targetArev = 465116.0*calc_U1();
  targetBrev = 465116.0*calc_U2();
  targetCrev = 465116.0*calc_U3();



  Serial.println("OmniBrain Online...");

  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);


  Wire.begin(3);                // join i2c bus with address #3
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(receiveRequest);
}



void receiveEvent(int howMany)
{

  char serialdata[5] = {0,0,0,0,0};
  serialdata[0] = Wire.read();
  serialdata[1] = Wire.read();
  serialdata[2] = Wire.read();
  serialdata[3] = Wire.read();
  serialdata[4] = Wire.read();

  parseSerialData(serialdata);

  // debugIntegerA = howMany;

  // Wire.println(phi);
 
}

void receiveRequest() {
  
  byte serialdata[17];
  serialdata[0] = tm_execute_target_motion; // batt status 0-255

  // current encoder values
  serialdata[1] = lowByte(encA);
  serialdata[2] = highByte(encA);

  serialdata[3] = lowByte(encB);
  serialdata[4] = highByte(encB);

  serialdata[5] = lowByte(encC);
  serialdata[6] = highByte(encC);

  int tempInt = 0;

  // current position values
  tempInt = round(current_Vx);

  serialdata[7] = lowByte(tempInt);
  serialdata[8] = highByte(tempInt);

  tempInt = round(current_Vy);

  serialdata[9] = lowByte(tempInt);
  serialdata[10] = highByte(tempInt);

  tempInt = round(current_omegaMini);

  serialdata[11] = lowByte(tempInt);
  serialdata[12] = highByte(tempInt);


  // target values
  tempInt = round(tm_target_x);

  serialdata[13] = lowByte(tempInt);
  serialdata[14] = highByte(tempInt);

  tempInt = round(tm_target_y);

  serialdata[15] = lowByte(tempInt);
  serialdata[16] = highByte(tempInt);

  
  Wire.write(serialdata,sizeof(serialdata));
}


void parseSerialData(char serialdata[5]) {
  switch(serialdata[0]) {
    case 0: 
      sendBackPong();
      break;
    case 1: 
      stopAllMotion();
      break;
    case 2: 
      executeMotion();
      break;
    case 3: 
      setPhi(serialdata);
      break;
    case 4: 
      moveTo(serialdata);
      break;
    case 5:
      clearCurrentPostion();
      break;
    case 6:
      setSpeed(serialdata);
  } 
}

// ----------------------------------
// Wire communication sub function

void sendBackPong() {

}

void stopAllMotion() {
  vectorX = 0;
  vectorY = 0;  
  vectorVelocity = 0;
  tm_execute_target_motion = 0;
}

void executeMotion() {
  tm_execute_target_motion = 1;  
}

void clearCurrentPostion() {
  current_Vx = 0.0;
  current_Vy = 0.0;
  current_omegaMini = 0.0;
  encA = 0;
  encB = 0;
  encC = 0;
}

void setPhi(char sd[5]) {
  int targetIntTemp = 0;
  char intTemp[2] = {0,0};

  intTemp[0] = sd[1]; 
  intTemp[1] = sd[2];
  targetIntTemp = *((int *)intTemp);
  phi = float(targetIntTemp)/100;
}

void yawRate(char sd[5]) {
  int targetIntTemp = 0;
  char intTemp[2] = {0,0};

  intTemp[0] = sd[1];
  intTemp[1] = sd[2];  
  targetIntTemp = *((int *)intTemp);
  yaw_rate = float(targetIntTemp)/100;
}

void moveTo(char sd[5]) {
  int targetIntTemp = 0;
  char intTemp[2] = {0,0};

  float h=0.0;
  float v=0.0;

  intTemp[0] = sd[1];
  intTemp[1] = sd[2];  
  targetIntTemp = *((int *)intTemp);
  tm_target_x = float(targetIntTemp);


  intTemp[0] = sd[3];
  intTemp[1] = sd[4];  
  targetIntTemp = *((int *)intTemp);
  tm_target_y = float(targetIntTemp);
}

void setSpeed(char sd[5]) {
  int targetIntTemp = 0;
  char intTemp[2] = {0,0};

  intTemp[0] = sd[1];
  intTemp[1] = sd[2];
  targetIntTemp = *((int *)intTemp);
  
  vectorVelocity = float(targetIntTemp)/100;
}

// end: Wire communication sub function
// ----------------------------------


void setVector(float x, float y) {
  vectorX = x;
  vectorY = y;
}





volatile byte motorADir = 0;
volatile byte motorBDir = 0;
volatile byte motorCDir = 0;


void loop() {

  float currentAVelocity = 0.0;
  float currentBVelocity = 0.0;
  float currentCVelocity = 0.0;

  if((sequenceCounter%500)==0 && tm_execute_target_motion==1) {
    // calculate current position to target position delta and set proper motion vector

    float tmp_deltaX = tm_target_x-current_Vx;
    float tmp_deltaY = tm_target_y-(current_Vy*-1.0);

    /*
    debugIntegerA = tmp_deltaY;
    debugIntegerB = tm_target_y;
    debugIntegerC = current_Vy;
    */


    

    float base = tmp_deltaY;

    float hM = 1.0;
    float vM = 1.0;

    if(tmp_deltaX<0.0) hM = -1;
    if(tmp_deltaY<0.0) vM = -1;

    if(abs(tmp_deltaX)>abs(tmp_deltaY)) base = tmp_deltaX;

    tmp_deltaX = abs(tmp_deltaX*5.0)/base;
    tmp_deltaY = abs(tmp_deltaY*5.0)/base;

    vectorX = tmp_deltaX*hM;
    vectorY = tmp_deltaY*vM;

    // tmp_deltaX = abs(tm_target_x-current_Vx);
    // tmp_deltaY = abs(tm_target_y+current_Vy);    

    tmp_deltaX = (tm_target_x-current_Vx);
    tmp_deltaY = (tm_target_y+current_Vy);    

    int tmp_target_delta_dist = sqrt((tmp_deltaX*tmp_deltaX)+(tmp_deltaY*tmp_deltaY));

    deltaXY = int(tm_target_x);

    debugIntegerA = tm_target_x;
    debugIntegerB = tm_target_y;
    debugIntegerC = tmp_target_delta_dist;

    

    // debugIntegerA = float(tmp_target_delta_dist);

    if(tmp_target_delta_dist<tm_end_zone) {
      tm_execute_target_motion = 0;
      stopAllMotion();
    }
  }

  if((sequenceCounter%250)==0) {
    // phi = phi+0.01;

    calc_Vel_Vector(vectorVelocity,0.0,vectorX,0.0,vectorY); // float V, float x0, float x1, float y0, float y1

    cosPhi = cos(phi);
    sinPhi = sin(phi);

    targetArev = 465116.0*calc_U1();
    targetBrev = 465116.0*calc_U2();
    targetCrev = 465116.0*calc_U3();
    // interrupts();  
  }

  
  
  if((sequenceCounter%5)==0) {
    // calc total wheel revolution
    encArad = encA * 0.04870686284635;
    encBrad = encB * 0.04870686284635;
    encCrad = encC * 0.04870686284635;

    float deltaArev = abs(targetArev)-encAVelocity;
    float deltaBrev = abs(targetBrev)-encBVelocity;
    float deltaCrev = abs(targetCrev)-encCVelocity;


    currentAVelocity = currentAVelocity + (deltaArev/5.0);
    currentBVelocity = currentBVelocity + (deltaBrev/5.0);
    currentCVelocity = currentCVelocity + (deltaCrev/5.0);

    // max rev limit
    if(currentAVelocity>maxRev) currentAVelocity = maxRev;
    if(currentBVelocity>maxRev) currentBVelocity = maxRev;
    if(currentCVelocity>maxRev) currentCVelocity = maxRev;

    // min rev limiter
    if(currentAVelocity<0.0) currentAVelocity = 0.0;
    if(currentBVelocity<0.0) currentBVelocity = 0.0;
    if(currentCVelocity<0.0) currentCVelocity = 0.0;
    
    // rev to motorrev value
    currentMSpeedA = minMotorRev+((maxMotorRev-minMotorRev)*(currentAVelocity/maxRev));
    currentMSpeedB = minMotorRev+((maxMotorRev-minMotorRev)*(currentBVelocity/maxRev));
    currentMSpeedC = minMotorRev+((maxMotorRev-minMotorRev)*(currentCVelocity/maxRev));

    if(currentMSpeedA<=minMotorRev) currentMSpeedA = 0.0;
    if(currentMSpeedB<=minMotorRev) currentMSpeedB = 0.0;
    if(currentMSpeedC<=minMotorRev) currentMSpeedC = 0.0;


    calcCurrentPos();

    // update motor speed
    
    byte doMotor = HIGH; // HIGH LOW
    if(doMotor==1) {
      if(currentMSpeedA>minMotorRev) {
        motorA.setSpeed(currentMSpeedA);
        if(targetArev>0.0) {
          motorA.run(FORWARD); 
          motorADir = 0;
        } else {
          motorA.run(BACKWARD);
          motorADir = 1;
        }
      } else {
        motorA.setSpeed(0);
        motorA.run(RELEASE);
      }

      if(currentMSpeedB>minMotorRev) {
        motorB.setSpeed(currentMSpeedB);
        if(targetBrev>0.0) {
          motorB.run(FORWARD);
          motorBDir = 0;
        } else {
          motorB.run(BACKWARD);
          motorBDir = 1;
        }
      } else {
        motorB.setSpeed(0);
        motorB.run(RELEASE);
      }

      if(currentMSpeedC>minMotorRev) {
        motorC.setSpeed(currentMSpeedC);
        if(targetCrev>0.0) {
          motorC.run(FORWARD);
          motorCDir = 0;
        } else {
          motorC.run(BACKWARD);
          motorCDir = 1;
        }
      } else {
        motorC.setSpeed(0);
        motorC.run(RELEASE);
      }
    } 
  }
  

  
  sequenceCounter++;
}

ISR(TIMER1_COMPA_vect) //TIMER1_COMPA_vect
{
  // digitalWrite(10,HIGH);
  
  int arEncATmp = analogRead(1);
  int arEncBTmp = analogRead(3);
  int arEncCTmp = analogRead(2);
  
  // digitalWrite(10,arEncCTmp);
  
  byte encCurAState = 0;
  byte encCurBState = 0;
  byte encCurCState = 0;

  
  if(arEncATmp>512) encCurAState = 1;
  if(arEncBTmp>512) encCurBState = 1;
  if(arEncCTmp>512) encCurCState = 1;

  unsigned long ct = micros(); // current time in micros

  // 465116.2791 = 60000000(micro in min)/129(encoder slot count x2)
  unsigned long tAdelta = ct-encALastT;
  unsigned long tBdelta = ct-encBLastT;
  unsigned long tCdelta = ct-encCLastT;

  if(encCurAState!=encAState && tAdelta<10000000L) {
    if(motorADir==0) encA++; else encA--;
    encAVelocity = 465116.0/(float)tAdelta;
    encALastT = ct;
  }
  if(encCurBState!=encBState && tBdelta<10000000L) {
    if(motorBDir==0) encB++; else encB--;
    encBVelocity = 465116.0/(float)tBdelta;
    encBLastT = ct;
  }
  if(encCurCState!=encCState && tCdelta<10000000L) {
    if(motorCDir==0) encC++; else encC--;
    encCVelocity = 465116.0/(float)tCdelta;
    encCLastT = ct;
  }
  
  // digitalWrite(10,HIGH);

  if(tAdelta>500000L) {
    encALastT = ct;
    encAVelocity = 0.0;
  }
  if(tBdelta>500000L) {
    encBLastT = ct;
    encBVelocity = 0.0;
  }
  if(tCdelta>500000L) {
    encCLastT = ct;
    encCVelocity = 0.0;
  }


  encAState = encCurAState;
  encBState = encCurBState;
  encCState = encCurCState;


  // digitalWrite(10,LOW);
}


// basic motor control function
void stopAll() {
  motorA.setSpeed(0);
  motorB.setSpeed(0);
  motorC.setSpeed(0);
  
  motorA.run(RELEASE);
  motorB.run(RELEASE);
  motorC.run(RELEASE);
}


// calc velocity vector
void calc_Vel_Vector(float V, float x0, float x1, float y0, float y1) {
  float deltaX = x1-x0;
  float deltaY = y1-y0;
  float tmp = sqrt((deltaX*deltaX)+(deltaY*deltaY));
  V_x_vector = (V*deltaX)/tmp;
  V_y_vector = (V*deltaY)/tmp;
}


// calc tire rev
float calc_U1() {
  return (1.0/(6.0*piR))*((-V_x_vector*cosPhi)-(V_y_vector*sinPhi)+(wheelCenterDistance*yaw_rate));
}

float calc_U2() {
  float sqrt3 = sqrt(3.0);
  return (1.0/(12.0*piR))*((V_x_vector*((sqrt3*sinPhi)+cosPhi)+V_y_vector*((-sqrt3*cosPhi)+sinPhi))+(wheelCenterDistance*yaw_rate));
}

float calc_U3() {
  float sqrt3 = sqrt(3.0);
  return (1.0/(12.0*piR))*((-V_x_vector*((sqrt3*sinPhi)-cosPhi)+V_y_vector*((sqrt3*cosPhi)+sinPhi))+(wheelCenterDistance*yaw_rate));
}


void calcCurrentPos() {
  // seite 13
  float U3 = encArad+encBrad+encCrad;
  float U2 = encBrad-encCrad;
  float sqrt3 = sqrt(3.0);
  current_Vx = piR/3 * ((-2 * U3)*cosPhi+(U2)*sqrt3*sinPhi);
  current_Vy = piR/3 * ((-2 * U3)*sinPhi+(U2)*sqrt3*cosPhi);
  current_omegaMini = ((2*piR)/(3*wheelCenterDistance)) * U3;
}

void calcCurrentPosB() {
  // float current_X = piR/3 * (()*cosPhi*);
}

// #######################################
// Debug functions. uncomment to use
