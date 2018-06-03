#include <Wire.h>
// #include <OmniBrainBrainUIU.h>

#define Stick_H 1
#define Stick_V 0

#define Servo_H 5
#define Servo_V 6



#define F_CPU 16000000
#define PRESCALER 64;
#define PRESCALER_BITS (1<<CS22) | (1<<CS20)

#define NR_SERVOS 2

// http://wiki.mikrotik.com/wiki/Super_Mario_Theme
int mukkeArray[87] = {
  660,100,150
,660,100,300
,660,100,300
,510,100,100
,660,100,300
,770,100,550
,380,100,575
,510,100,450
,380,100,400
,320,100,500
,440,100,300
,480,80,330
,450,100,150
,430,100,300
,380,100,200
,660,80,200
,760,50,150
,860,100,300
,700,80,150
,760,50,350
,660,80,300
,520,80,150
,580,80,150
,480,80,500
,510,100,450
,380,100,400
,320,100,500
,440,100,300
,480,80,330};

// servo controll
volatile int ServoValues[NR_SERVOS];
volatile int ServoIDS[NR_SERVOS];

int MAX_H_PERIMETER = 140;

int SERVO_CENTER_V = 140;
int SERVO_CENTER_H = 147;

int MILLISEC_BASE = 0;
int SERVO_CENTER = 0;

int calb_groundlevel = 0;

int stickHSamples[5] = {0,0,0,0,0};
int stickVSamples[5] = {0,0,0,0,0};

int stickH = 0;
int stickV = 0;

int stickHCenter = 0;
int stickVCenter = 0;

int stickHThreshold = 2; // default threshold value
int stickVThreshold = 2; // default threshold value

int maxStickSpeedH = 10;
int maxStickSpeedV = 10;

int botMode = -1;

int botMoveVectorX = 0;
int botMoveVectorY = 0;
int botMovePhi = 0;

byte servoLock = B00000000;

//#############################################################################
//#############################################################################
// UIU vriables
//#############################################################################

// UI Shiftregister unit pin configuration
#define SREG_DS   9 // shift register datainput
#define SREG_OE   8 // shift register output enable
#define SREG_ST   7 // shift register storage register clock
#define SREG_MR   4 // shift register master reset
#define SREG_SH   3 // shift register clock

#define PIN_UIU_BTN_IN 2
#define PIN_UIU_BUZZER 11


// shiftregister array index matrix
#define SREG_INDEX_LED_BTN_Red 0
#define SREG_INDEX_LED_BTN_Blue 8

#define SREG_INDEX_LED_BAR_1 1
#define SREG_INDEX_LED_BAR_2 2
#define SREG_INDEX_LED_BAR_3 3
#define SREG_INDEX_LED_BAR_4 4
#define SREG_INDEX_LED_BAR_5 5
#define SREG_INDEX_LED_BAR_6 6
#define SREG_INDEX_LED_BAR_7 7
#define SREG_INDEX_LED_BAR_8 9
#define SREG_INDEX_LED_BAR_9 11

#define SREG_INDEX_BUTTON_Red 15
#define SREG_INDEX_BUTTON_Blue 10

#define SREG_INDEX_PROGRAMM_SW_1 14
#define SREG_INDEX_PROGRAMM_SW_2 13
#define SREG_INDEX_PROGRAMM_SW_3 12

byte UIUshiftRegisterBuffer[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

byte UIULedModes[11] = {4,4,1,2,3,4,5,4,4,4,4};

byte UIULedMatrix[11] = {
  SREG_INDEX_LED_BTN_Red,
  SREG_INDEX_LED_BTN_Blue,
  SREG_INDEX_LED_BAR_1,
  SREG_INDEX_LED_BAR_2,
  SREG_INDEX_LED_BAR_3,
  SREG_INDEX_LED_BAR_4,
  SREG_INDEX_LED_BAR_5,
  SREG_INDEX_LED_BAR_6,
  SREG_INDEX_LED_BAR_7,
  SREG_INDEX_LED_BAR_8,
  SREG_INDEX_LED_BAR_9
};

byte UIUButtonState[5] = {4,0,0,0,0};
byte UIUButtonMatrix[5] = {
  SREG_INDEX_BUTTON_Red,
  SREG_INDEX_BUTTON_Blue,
  SREG_INDEX_PROGRAMM_SW_1,
  SREG_INDEX_PROGRAMM_SW_2,
  SREG_INDEX_PROGRAMM_SW_3};

byte UIUBuzzerSound = 0;
int UIUBuzzerSoundCounter = 0;

void setup(void) {
  

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  

  MILLISEC_BASE = F_CPU/64/1000;
  SERVO_CENTER = MILLISEC_BASE / 2;
  
  pinMode(Servo_H, OUTPUT);
  pinMode(Servo_V, OUTPUT);

  InitServo();

  clibrateStick();

  UIShiftRegisterSetup(); // init UIU

  delay(1000);
  Wire.begin(); // join I2C bus as master.
  Serial.begin(19200);
  Serial.println("system online...");
}

boolean serialMsg = false;

void clibrateStick() {
  stickHSamples[0]=analogRead(Stick_H);
  Serial.println(stickHSamples[0]);
  stickHSamples[1]=analogRead(Stick_H);
  Serial.println(stickHSamples[1]);
  stickHSamples[2]=analogRead(Stick_H);
  Serial.println(stickHSamples[2]);
  stickHSamples[3]=analogRead(Stick_H);
  Serial.println(stickHSamples[3]);
  stickHSamples[4]=analogRead(Stick_H);
  Serial.println(stickHSamples[4]);
  stickH = (stickHSamples[0]+stickHSamples[1]+stickHSamples[2]+stickHSamples[3]+stickHSamples[4])/5;
  Serial.println(stickH);

  stickVSamples[0]=analogRead(Stick_V);
  Serial.println(stickVSamples[0]);
  stickVSamples[1]=analogRead(Stick_V);
  Serial.println(stickVSamples[1]);
  stickVSamples[2]=analogRead(Stick_V);
  Serial.println(stickVSamples[2]);
  stickVSamples[3]=analogRead(Stick_V);
  Serial.println(stickVSamples[3]);
  stickVSamples[4]=analogRead(Stick_V);
  Serial.println(stickVSamples[4]);
  stickV = (stickVSamples[0]+stickVSamples[1]+stickVSamples[2]+stickVSamples[3]+stickVSamples[4])/5;
  Serial.println(stickV);

  stickHCenter = stickH;
  stickVCenter = stickV;  
}

void InitServo() {
  noInterrupts();
  ServoIDS[0] = Servo_H;
  ServoIDS[1] = Servo_V;

  int i = 0;
  for(i=0; i<NR_SERVOS; i++)
    ServoValues[i] = SERVO_CENTER;    

  // interrup setup
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = MILLISEC_BASE + ServoValues[0];
  TIMSK1 |= (1<<OCIE1A);
  TCCR1B |= (1<<WGM12);
  TCCR1B |= (1<<CS11); // CTC mode
  TCCR1B |= (1<<CS10); // CTC mode
  interrupts();
}


byte stickSampleCounter = 0;

byte dutyCircle_footBrainCom = 0;
byte modeReset = 0;

void loop(void) {
  serialMsg = false;

  // digitalWrite(10,HIGH); // debug pin out
  
  if(botMode==-1) modeCalibrateWaitMode(); // not clibrated, wait for calibration start

  if(botMode==0) modeSelectMode();
  
  if(dutyCircle_footBrainCom%50==0) {
    /*
    programm codes
    100 stickCalibrationMotion
    010 full automatic mode
    110 doStickBackOffMotion
    001 dog leash mode
    101 stickScanMotion
    011 play Music
    */

    stickPositionReadSampler(); // read analog joystick values

    // 100
    if(botMode==1) stickCalibrationMotion(); // do initial calibration

    // 010
    if(botMode==2) { // full automatic mode
      
      MotionPlaner();
    }

    // 110
    if(botMode==3) doStickBackOffMotion(3);

    // 001
    if(botMode==4) {
      doStickMotion(0,-150,2,2);
      botMoveVectorX = stickHPos();
      botMoveVectorY = stickVPos();
      // botMovePhi = stickHPos();
      sendDataToFootBrain();
    }

    // 101
    if(botMode==5) stickScanMotion(); // do scan motion only

    // 011
    if(botMode==6) playMukke();

    // 111
    if(botMode==7) doStickBackOffMotion(10);


    if(UIUButtonState[0]==1) {
      // red button pressed
      char serialdata[5] = {0,0,0,0,0};
      serialdata[0] = 1;
      sendWirePacket(serialdata);
      botMode = 0;
    }
  }
  modeReset = 0;
  

  // digitalWrite(10,LOW);  // debug pin out
  if(serialMsg) Serial.println();
  
  dutyCircle_footBrainCom++;

  UIUMasterControler();
}

void MotionPlaner() {
  // check stick status
  // option A: if stick detects gap or obstacle, drive back until no gap or obstacle is detected and make a 20deg turn left and try to drive forward

  // option B: if stick detects gap or obstacle, try to detect linear gap or obstacle line and rotate in counter direction until no gap or obstacle is detected

  // if stick status normal drive forward

  static int lastStickHPos = 0;

  byte scanResult = stickScanMotion();
  if(scanResult==0) {    
    botMoveVectorY = 50;
    botMovePhi = 0;
    sendDataToFootBrain();
  }
  if(scanResult==1) {
    botMoveVectorY = 0;
    botMovePhi = 0;
    lastStickHPos = stickHPos();
    sendDataToFootBrain();
  }
  if(scanResult==2) {
    botMoveVectorX = 0;
    botMoveVectorY = -50;  // -70
    botMovePhi = 25;
    // botMovePhi = lastStickHPos*10;
    sendDataToFootBrain();
  }
  
}

void sendDataToFootBrain() {

  int dataInt = botMoveVectorX;
  dataInt = (dataInt/4)+130;
  if(dataInt>255) dataInt = 255;
  if(dataInt<0) dataInt = 0;
  byte dataHPos = byte(dataInt);

  dataInt = botMoveVectorY;
  dataInt = (dataInt/4)+130;
  if(dataInt>255) dataInt = 255;
  if(dataInt<0) dataInt = 0;
  byte dataVPos = byte(dataInt);

  dataInt = botMovePhi;
  dataInt = (dataInt/4)+130;
  if(dataInt>255) dataInt = 255;
  if(dataInt<0) dataInt = 0;
  byte dataPhi = byte(dataInt);

  char serialdata[5] = {0,0,0,0,0};
  serialdata[0] = 4;
  serialdata[1] = lowByte(botMoveVectorX); // botMoveVectorX
  serialdata[2] = highByte(botMoveVectorX);
  serialdata[3] = lowByte(botMoveVectorY); // botMoveVectorY
  serialdata[4] = highByte(botMoveVectorY);

  sendWirePacket(serialdata);

  serialdata[0] = 3;
  serialdata[1] = lowByte(botMovePhi);
  serialdata[2] = highByte(botMovePhi);
  serialdata[3] = 0;
  serialdata[4] = 0;

  sendWirePacket(serialdata);
}

void sendWirePacket(char sd[5]) {
  servoLock=B00000001; // enable servo lock
  delay(1);
  
  // sending motion command via i2c
  Wire.beginTransmission(3);
  Wire.write(sd[0]);     
  Wire.write(sd[1]);
  Wire.write(sd[2]);
  Wire.write(sd[3]);
  Wire.write(sd[4]);
  Wire.endTransmission();
  
  servoLock=B00000000; // release servo lock
}

void stickPositionReadSampler() {
  // read analog joystick values
  stickHSamples[stickSampleCounter]=analogRead(Stick_H);
  if(stickSampleCounter==2) {
    stickH = (stickHSamples[0]+stickHSamples[1]+stickHSamples[2])/3;
  }

  stickVSamples[stickSampleCounter]=analogRead(Stick_V);
  if(stickSampleCounter==2) {
    stickV = (stickVSamples[0]+stickVSamples[1]+stickVSamples[2])/3;
  }

  stickSampleCounter++;
  if(stickSampleCounter>2) stickSampleCounter = 0;
}

void serialOutStickInfo() {
  serialMsg = true;
  Serial.print("stick: [");
  Serial.print(stickHPos());
  Serial.print(" - ");
  Serial.print(stickVPos());
  Serial.print("] ");
}

void serialOutServoInfo() {
  serialMsg = true;
  Serial.print("servo: [");
  Serial.print(ServoValues[0]);
  Serial.print(" - ");
  Serial.print(ServoValues[1]);
  Serial.print("] ");
}

void stickCalibrationMotion() {
  static int calibMode = 0;
  static int calibSV1 = 0;
  static int calibSV2 = 0;
  static int servoH = 0;
  static int servoV = 0;

  static int freq = 10;
  static int maxStickVal = 0;
  static int minStickVal = 1000;

  if(modeReset==1) {
    calibMode = 0;
    calibSV1 = 0;
    calibSV2 = 0;
    servoH = 0;
    servoV = 0;
    freq = 10;
    maxStickVal = 0;
    minStickVal = 1000;
  }

  byte motionStatus = 0;
  
  int target = 0;

  if(calibMode==0) {
    // go to center position and calibrate center position of sensor stick
    
    servoV = SERVO_CENTER_V;
    servoH = SERVO_CENTER_H;
    ServoValues[0]=servoV; // 140
    ServoValues[1]=servoH; // 147
    
    serialMsg = true;
    Serial.println("move to center");

    delay(2000);

    clibrateStick();

    Serial.print("center ");
    serialOutStickInfo();
    Serial.println();

    calibMode = 1;
  }
  
  if(calibMode==1) {
    // goto upper right for ground level calibration
    motionStatus = doStickMotion(120,0,2,2);
    if(motionStatus==13) {
      Serial.println("move 1 done");
      calibMode = 2;
    } else {
      doStickBackOffMotion(10);
    }
  }

  
  
  if(calibMode==2) {
    // calibrate right ground level
    motionStatus = doStickMotion(120,140,0,2);
    if(motionStatus==13) {
      // no contact with ground. repeat right ground level calibration
      calibMode = 1;
      Serial.println("right calibration error! no contact with ground! recalibrate");
      UIUStartBuzzerSound(2);
    } else if(motionStatus>20) {
      // touch down proceed with left ground level calibration
      
      if(motionStatus==22) {
        Serial.println("right calibration done");
        calibSV1 = servoV;
        calibMode=201;
        UIUStartBuzzerSound(5);
      } else {
        Serial.println("right calibration error! horizontal contact! recalibrate");
        UIUStartBuzzerSound(2);
        calibMode = 1;
      }
    }
  }

  if(calibMode==201) {
    // do back off motion
    motionStatus = doStickBackOffMotion(2);
    if(motionStatus==11) {
      Serial.println("move 201 done");
      calibMode=202;
    }
  }

  if(calibMode==202) {
    // go to upper right position
    motionStatus = doStickMotion(120,0,0,2);
    if(motionStatus==13) {
      Serial.println("move 202 done");
      calibMode=203;
    }
  }

  if(calibMode==203) {
    // go to upper left position
    motionStatus = doStickMotion(-120,0,2,2);
    if(motionStatus==13) {
      Serial.println("move 203 done");
      calibMode=3;
    } else if(motionStatus>20) {
      doStickBackOffMotion(10);
    }
  }

  if(calibMode==3) {
    // calibrate left ground level
    motionStatus = doStickMotion(-120,140,0,2);
    if(motionStatus==13) {
      Serial.println("left calibration error! no contact with ground! recalibrate");
      UIUStartBuzzerSound(2);
      calibMode = 203;
    } else if(motionStatus>20) {
      Serial.println(motionStatus);
      if(motionStatus==22) {
        calibSV2 = servoV;

        Serial.println();
        Serial.print("calib done ");
        Serial.print(calibSV1);
        Serial.print(" - ");
        Serial.print(calibSV2);
        calb_groundlevel = (calibSV1+calibSV2)/2;


        Serial.print(" groundLvL: ");
        Serial.println(calb_groundlevel);
        UIUStartBuzzerSound(5);
        calibMode=301;
      } else {
        Serial.println("left calibration error! horizontal contact! recalibrate");
        UIUStartBuzzerSound(2);
        calibMode = 203;
      }
    }
  }
  

  if(calibMode==301) {
    motionStatus = doStickBackOffMotion(2);
    if(motionStatus==11) {
      Serial.println("move 301 done");
      calibMode=302;
    }
  }

  if(calibMode==302) {
    motionStatus = doStickMotion(0,0,2,2);
    if(motionStatus==13) {
      Serial.println("move 302 done");
      UIUStartBuzzerSound(3);
      calibMode = 0;
      botMode = 0;
    }
  }
}

byte stickScanMotion() {  
  static int motionMode = 0;
  static byte scanStatus = 0;

  int detectDown = calb_groundlevel+7;
  int detectUp = calb_groundlevel-60;

  int leftOuterPos = 80;
  int rightOuterPos = -leftOuterPos;
  static int outerPosOffset = 0;

  byte motionStatus = 0;
  byte motionModeReturn = 0;

  
  if(motionMode==0) {
    motionStatus = doStickMotion(leftOuterPos+outerPosOffset,detectUp,5,5);
    if(motionStatus==13) {
      motionMode = 1;
    } else if(motionStatus>20) {
      doStickBackOffMotion(5);
    }
  }

  if(motionMode==1) {
    motionStatus = doStickMotion(leftOuterPos+outerPosOffset,detectDown,5,2);
    if(motionStatus==13) {
      
      motionMode = 0;
      
      if(rightOuterPos>(leftOuterPos+outerPosOffset)) {
        scanStatus = 2;
      } else {
        outerPosOffset -= 30;
        scanStatus = 1;
      }
      
    } else if(motionStatus>20) {
      motionMode = 101;
      outerPosOffset = 0;
      scanStatus = 0;
    }
  }

  if(motionMode==101) {
    motionStatus = doStickBackOffMotion(5);
    if(motionStatus==11) {
      motionMode = 2;
    }
  }

  if(motionMode==2) {
    motionStatus = doStickMotion(rightOuterPos+outerPosOffset,detectUp,5,5);
    if(motionStatus==13) {
      motionMode = 3;
    } else if(motionStatus>20) {
      doStickBackOffMotion(5);
    }
  }

  if(motionMode==3) {
    motionStatus = doStickMotion(rightOuterPos+outerPosOffset,detectDown,5,2);
    if(motionStatus==13) { 
      motionMode = 2;

      if(leftOuterPos<(rightOuterPos+outerPosOffset)) {
        scanStatus = 2;
      } else {
        outerPosOffset += 30;
        scanStatus = 1;
      }
    } else if(motionStatus>20) {
      outerPosOffset = 0;
      motionMode=4;
      scanStatus = 0;
    }
  }

  if(motionMode==4) {
    motionStatus = doStickBackOffMotion(5);
    if(motionStatus==11) {
      motionMode = 0;
    }
  }

  // serialOutStickInfo();

  // serialOutServoInfo();
  return scanStatus;
}

// -------------------------------------------------------------
// basic servo motion routines
byte doStickMotion(int targetH,int targetV,int speedH, int speedV) {
  /* 
  0 = executing motion
  11 = h target postion reached
  12 = v target postion reached
  13 = h & v target position reached
  21 = while moving to h target blocked
  22 = while moving to v target blocked
  */
  byte motionResult = 0; 
  
  int deltaH = 0;
  int deltaV = 0;

  int stepH = 0;
  int stepV = 0;

  targetH = targetH+SERVO_CENTER_H;
  targetV = targetV+SERVO_CENTER_V;

  deltaH = targetH-ServoValues[1];
  if(abs(deltaH)<speedH) {
    stepH = 0;
    ServoValues[1] = targetH;
    motionResult = 11;
  } else {
    if(deltaH>0) 
      stepH = speedH;
    else
      stepH = -speedH;
  }

  deltaV = targetV-ServoValues[0];
  if(abs(deltaV)<speedV) {
    stepV = 0;
    ServoValues[0] = targetV;
    motionResult = 12;
  } else {
    if(deltaV>0) 
      stepV = speedV;
    else
      stepV = -speedV;
  }

  if(abs(deltaV)==0 && abs(deltaH)==0) {
    motionResult = 13;
  }

  // check stick collision
  if(stickHThresholdPass()!=0) motionResult = 21;
  if(stickVThresholdPass()!=0) motionResult = 22;

  // change servo value only if required
  
  if(motionResult<13) {
    ServoValues[1]+=stepH;
    ServoValues[0]+=stepV;  
  }

  return motionResult;
}

byte doStickBackOffMotion(int speed) {
  /*
  stick back off motion. move the stick to the opposite blocking direction.
  
  0 = executing motion
  11 = h & v position reached
  */

  byte motionResult = 0; 
  

  int stepH = 0;
  int stepV = 0;

  // check stick collision
  byte shtp = stickHThresholdPass();
  byte svtp = stickVThresholdPass();

  if(shtp!=0) {
    if(shtp==1) {
      stepH = speed;
    } else {
      stepH = -speed;
    }
  }

  if(svtp!=0) {
    if(svtp==1) {
      stepV = speed;
    } else {
      stepV = -speed;
    }
  }

  if(shtp==0 && svtp==0) {
    motionResult = 11;
  }

  // change servo value only if required
  if(motionResult==0) {
    ServoValues[1]+=stepH;
    ServoValues[0]+=stepV;  
  }

  return motionResult;
}


ISR(TIMER1_COMPA_vect) //TIMER1_COMPA_vect
{
  // main servo puls interrupt routine
  if(servoLock!=B00000011) { // execute only if servo unlocked. prevent generating incorrect servo impulse.  
    static int CurrentServoID = 0;
    digitalWrite(10,HIGH); // set debug pin to high
    digitalWrite(ServoIDS[CurrentServoID],LOW); // set previews servo pin to low

    if(++CurrentServoID>=NR_SERVOS) {
      CurrentServoID = 0; // next servo id increment and bound
      if(servoLock==B00000001) servoLock=B00000011;
    }

    if(servoLock==B00000000) {
      digitalWrite(ServoIDS[CurrentServoID],HIGH); // set current servo pin to high
      OCR1A = MILLISEC_BASE + ServoValues[CurrentServoID]; // set timer interrupt to set current servo pin to low
      digitalWrite(10,LOW); // set debug pin to low  
    }
    
  }
}

int stickHPos() {
  return stickH-stickHCenter; 
}

int stickVPos() {
  return stickV-stickVCenter; 
}

byte stickHThresholdPass() {
  byte result = 0;
  int stickPos = stickH-stickHCenter;
  if(abs(stickPos)>stickHThreshold) {
    if(stickPos>0) result = 1;
    if(stickPos<0) result = 2;
  }
  return result;
}

byte stickVThresholdPass() {
  byte result = 0;
  int stickPos = stickV-stickVCenter;
  if(abs(stickPos)>stickVThreshold) {
    if(stickPos>0) result = 1;
    if(stickPos<0) result = 2;
  }
  return result;
}




// -------------------------------------------------------------
// -------------------------------------------------------------
// -------------------------------------------------------------
// UIU functions


void modeCalibrateWaitMode() {
  UIULedModes[0] = 1;
  UIULedModes[1] = 4;
  UIULedModes[2] = 0;
  UIULedModes[3] = 0;

  UIULedModes[4] = 0;
  UIULedModes[5] = 0;
  UIULedModes[6] = 0;
  UIULedModes[7] = 0;

  UIULedModes[8] = 0;
  UIULedModes[9] = 0;
  UIULedModes[10] = 0;
  UIULedModes[11] = 0;

  if(UIUButtonState[1]==1) {
    botMode = 1;
  }
}

void modeSelectMode() {
  // main menu function
  byte sw = 0;

  // set LED to default menu values
  UIULedModes[0] = 1;
  UIULedModes[1] = 4;
  UIULedModes[2] = 0;
  UIULedModes[3] = 0;

  UIULedModes[4] = 0;
  UIULedModes[5] = 0;
  UIULedModes[6] = 0;
  UIULedModes[7] = 0;

  UIULedModes[8] = 0;
  UIULedModes[9] = 0;
  UIULedModes[10] = 0;
  UIULedModes[11] = 0;

  // set switch bit 
  if(UIUButtonState[2]==1) sw = sw | B00000001;
  if(UIUButtonState[3]==1) sw = sw | B00000010;
  if(UIUButtonState[4]==1) sw = sw | B00000100;

  if(UIUButtonState[1]==1) {
    // if blue enter button pressed start selected programm mode
    botMode = sw;
    UIULedModes[0] = 4;
    UIULedModes[1] = 2;
    if(sw!=0) {
      sw = sw + 1;
      UIULedModes[sw] = 2;
    }
    modeReset = 1;
  } else if(sw!=0){
    // else just set selection LED to blink
    sw = sw + 1;
    UIULedModes[sw] = 4;
  }
}

void  UIShiftRegisterSetup() {
  // set buzzer and button pin mode
  pinMode(PIN_UIU_BUZZER, OUTPUT);
  pinMode(PIN_UIU_BTN_IN, INPUT);

  // set shiftregister pin
  pinMode(SREG_DS, OUTPUT);
  pinMode(SREG_MR, OUTPUT);
  pinMode(SREG_SH, OUTPUT);
  pinMode(SREG_ST, OUTPUT);
  pinMode(SREG_OE, OUTPUT);

  // init shiftregister
  digitalWrite(SREG_DS, LOW); //LOW active
  digitalWrite(SREG_MR, HIGH); //LOW active
  digitalWrite(SREG_SH, LOW); //LOW active
  digitalWrite(SREG_ST, LOW); //LOW active
  digitalWrite(SREG_OE, LOW); //LOW active

  // reset shift register
  digitalWrite(SREG_OE, HIGH); //LOW active
  digitalWrite(SREG_MR, LOW); //LOW active
  digitalWrite(SREG_OE, LOW); //LOW active
  digitalWrite(SREG_MR, HIGH); //LOW active

  digitalWrite(SREG_DS,LOW);
  digitalWrite(SREG_SH,LOW);
  digitalWrite(SREG_ST,LOW);
}

void UIUPushRegister() {
  byte iInv = 0;
  digitalWrite(SREG_DS,LOW);
  digitalWrite(SREG_SH,LOW);
  digitalWrite(SREG_ST,LOW);

  
  for(byte i=0; i<16; i++) {
    digitalWrite(SREG_DS,LOW);
    iInv = 15-i;
    if(UIUshiftRegisterBuffer[iInv]==1) digitalWrite(SREG_DS,HIGH);
    digitalWrite(SREG_SH,HIGH);
    digitalWrite(SREG_SH,LOW);
  }
  digitalWrite(SREG_ST,LOW);
  digitalWrite(SREG_ST,HIGH);
}


void UIUMasterControler() {
  
  static byte switchRegister=0;
  

  // low to all button
  UIUshiftRegisterBuffer[SREG_INDEX_BUTTON_Blue] = 0;
  UIUshiftRegisterBuffer[SREG_INDEX_PROGRAMM_SW_1] = 0;
  UIUshiftRegisterBuffer[SREG_INDEX_PROGRAMM_SW_2] = 0;
  UIUshiftRegisterBuffer[SREG_INDEX_PROGRAMM_SW_3] = 0;
  UIUshiftRegisterBuffer[SREG_INDEX_BUTTON_Red] = 0;

  switch(switchRegister) {
    case 0: UIUshiftRegisterBuffer[SREG_INDEX_BUTTON_Blue] = 1; break;
    case 2: UIUshiftRegisterBuffer[SREG_INDEX_BUTTON_Red] = 1; break;
    case 4: UIUshiftRegisterBuffer[SREG_INDEX_PROGRAMM_SW_1] = 1; break;
    case 6: UIUshiftRegisterBuffer[SREG_INDEX_PROGRAMM_SW_2] = 1; break;
    case 8: UIUshiftRegisterBuffer[SREG_INDEX_PROGRAMM_SW_3] = 1; break;
  }



  UIUPushRegister(); 

  byte buttonState = digitalRead(PIN_UIU_BTN_IN);


  if(buttonState && switchRegister==2) {
    UIUButtonState[0] = 1;
    UIUStartBuzzerSound(3);
  } else if(switchRegister==2){
    UIUButtonState[0] = 0;
  }

  if(buttonState && switchRegister==0) {
    UIUButtonState[1] = 1;
    UIUStartBuzzerSound(4);
  } else if(switchRegister==0) {
    UIUButtonState[1] = 0;
  }


  if(buttonState && switchRegister==4) {
    UIUButtonState[2] = 1;
  } else if(switchRegister==4) {
    UIUButtonState[2] = 0;
  }


  if(buttonState && switchRegister==6) {
    UIUButtonState[3] = 1;
  } else if(switchRegister==6) {
    UIUButtonState[3] = 0;
  }

  if(buttonState && switchRegister==8) {
    UIUButtonState[4] = 1;
  } else if(switchRegister==8) {
    UIUButtonState[4] = 0;
  }

  switchRegister++;
  if(switchRegister>20) switchRegister = 0;

  UIULedControler();

  if(UIUBuzzerSound>0) UIUDoBuzzerSound();
}

// -------------------------------------------------------------
// sound subroutines

void UIUStartBuzzerSound(byte sound) {
  if(UIUBuzzerSound==0) {
    switch(sound) {
      case 1: UIUBuzzerSoundCounter = 200; break;
      case 2: UIUBuzzerSoundCounter = 1000; break;
      case 3: UIUBuzzerSoundCounter = 750; break;
      case 4: UIUBuzzerSoundCounter = 1500; break;
      case 5: UIUBuzzerSoundCounter = 500; break;
      case 6: UIUBuzzerSoundCounter = 2000; break;
    }
    UIUBuzzerSound = sound;  
  }
}

void UIUDoBuzzerSound() {
  switch(UIUBuzzerSound) {
    case 1: UIUBuzzerSoundBeep(1000); break;
    case 2: UIUBuzzerSoundBeep(50); break;
    case 3: UIUBuzzerRandom(5); break;
    case 4: UIUBuzzerRandom(10); break;
    case 5: UIUBuzzerRandom(20); break;
    case 6: UIUBuzzerPiu(); break;
  }
}


void UIUBuzzerSoundBeep(int freq) {
  if(UIUBuzzerSoundCounter%50==0) tone(PIN_UIU_BUZZER,freq,20);
  UIUBuzzerSoundCounter--;
  if(UIUBuzzerSoundCounter<0) UIUBuzzerSound = 0;
}

void UIUBuzzerPiu() {
  static int height=300;
  if(UIUBuzzerSoundCounter%5==0) {
    height-=1;
    if(height<10) height = 300;
    int t = random(5,height);
    t = t*10;
    tone(PIN_UIU_BUZZER,t,5);
  }
  UIUBuzzerSoundCounter--;
  if(UIUBuzzerSoundCounter<0) UIUBuzzerSound = 0;
}

void UIUBuzzerRandom(int baseMultiplyer) {
  if(UIUBuzzerSoundCounter%250==0) {
    int t = random(50,100);
    t = t*baseMultiplyer;
    tone(PIN_UIU_BUZZER,t,50);
  }
  UIUBuzzerSoundCounter--;
  if(UIUBuzzerSoundCounter<0) UIUBuzzerSound = 0;
}

void playMukke() {
  byte mukkeLength = 29; // 156
  int mukkeFreq = 0;
  int duration = 0;
  int pause = 0;
  int offset = 0;

  for(byte i=0; i<mukkeLength; i++) {
    offset = 3*i;

    mukkeFreq = mukkeArray[offset];
    duration = mukkeArray[offset+1];
    pause = mukkeArray[offset+2];

    tone(PIN_UIU_BUZZER,mukkeFreq,duration);
    // delayMicroseconds(pause);
    delay(pause);
  }
  botMode = 0; 
}

// end: sound subroutines
// -------------------------------------------------------------



// -------------------------------------------------------------
// LED blink pattern subroutines
void UIULedControler() {
  static byte ledIndex = 0;
  static int UIU_master_led_counter = 0;

  byte LEDMode = UIULedModes[ledIndex];


  // Serial.println(UIU_master_led_counter);

  switch(LEDMode) {
    case 0: UIUshiftRegisterBuffer[UIULedMatrix[ledIndex]] = 0; break;
    case 1: UIUshiftRegisterBuffer[UIULedMatrix[ledIndex]] = 1; break;
    case 2: 
      if(UIU_master_led_counter%25==0) UIUshiftRegisterBuffer[UIULedMatrix[ledIndex]] = 1; 
      if(UIU_master_led_counter%50==0) UIUshiftRegisterBuffer[UIULedMatrix[ledIndex]] = 0; 
      break;
    case 3: 
      if(UIU_master_led_counter%100==0) UIUshiftRegisterBuffer[UIULedMatrix[ledIndex]] = 1; 
      if(UIU_master_led_counter%200==0) UIUshiftRegisterBuffer[UIULedMatrix[ledIndex]] = 0; 
      break;
    case 4: 
      if(UIU_master_led_counter%200==0) UIUshiftRegisterBuffer[UIULedMatrix[ledIndex]] = 1; 
      if(UIU_master_led_counter%400==0) UIUshiftRegisterBuffer[UIULedMatrix[ledIndex]] = 0; 
      break;
    case 5: 
      if(UIU_master_led_counter%200==0) UIUshiftRegisterBuffer[UIULedMatrix[ledIndex]] = 1; 
      if(UIU_master_led_counter%300==0) UIUshiftRegisterBuffer[UIULedMatrix[ledIndex]] = 0; 
      break;
  }

  ledIndex++;
  if(ledIndex==12) {
    UIU_master_led_counter++;
    ledIndex = 0;
  }
}

// END: LED blink pattern subroutines
// -------------------------------------------------------------

