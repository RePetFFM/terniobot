

#ifndef OmniBrainBrainUIU
#define OmniBrainBrainUIU
#include "Arduino.h"
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

class OmniBrainBrainUIU {
  private:
    byte UIUshiftRegisterBuffer[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    byte UIULedModes[11] = {4,4,4,4,4,4,4,4,4,4,4};

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


  public:


    OmniBrainBrainUIU();

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


    void UIUMasterControler() {
      byte buzzOn = 0;

      static byte switchRegister=0;
      switchRegister++;
      

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

      buzzOn = 0;

      byte buttonState = digitalRead(PIN_UIU_BTN_IN);


      if(buttonState && switchRegister==2) {
        UIUButtonState[0] = 1;
        buzzOn = 1;
      } else if(switchRegister==2){
        UIUButtonState[0] = 0;
      }

      if(buttonState && switchRegister==0) {
        UIUButtonState[1] = 1;
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

      if(switchRegister>20) switchRegister = 0;


      if(buzzOn==1) {
        /*
        digitalWrite(9,HIGH);
        delayMicroseconds(500);
        digitalWrite(9,LOW);
        */

        /*
        for(byte ic=500; ic<5000; ic++) {
          tone(PIN_UIU_BUZZER,ic);
          delay(1);
        }
        */
        UIUBuzzerRandom();
      }

      UIULedControler();
    }


    void UIUBuzzerRandom() {
      static byte counter=0;
      if(counter%10==0) {
        int t = random(10,300);
        t = t*10;
        tone(PIN_UIU_BUZZER,t,100);
      }
      counter++;
    }


    void UIULedControler() {
      static byte ledIndex = 0;
      static int UIU_master_led_counter = 0;

      byte LEDMode = UIULedModes[ledIndex];

      switch(LEDMode) {
        case 0: UIUshiftRegisterBuffer[UIULedMatrix[ledIndex]] = 0; break;
        case 1: UIUshiftRegisterBuffer[UIULedMatrix[ledIndex]] = 1; break;
        case 2: 
          if(UIU_master_led_counter%50==0) UIUshiftRegisterBuffer[UIULedMatrix[ledIndex]] = 1; 
          if(UIU_master_led_counter%100==0) UIUshiftRegisterBuffer[UIULedMatrix[ledIndex]] = 0; 
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
      if(ledIndex==11) {
        UIU_master_led_counter++;
        ledIndex = 0;
      }
    }


    void UIUPushRegister() {
      byte iInv = 0;
      // digitalWrite(SREG_OE, HIGH);
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

      // digitalWrite(SREG_OE, LOW);
    }
};

#endif