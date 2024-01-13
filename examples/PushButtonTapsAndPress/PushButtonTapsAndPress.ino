/*
  Author: Prashant Kumar

  Push Button Taps And Press
    - Arduino library to read Push Button as Single Tap, Double Tap or Long Press
      without using while statements or stopping program flow
    - setup single push button with a pull-up or pull-down 10K resistor 
	  and a 100nF decoupling capacitor to remove button bounce.
	- setup button as active high or low
    - Example: Active Low schematic:
              MCU Pin ----------------\
	          5V/3.3V ------[10K]------\
                                /-------\
                            [100nF]      ]O[
                                \-------/
              GND ---------------------/					  
    - classifies taps as one of 4 types (enum class ButtonTapType)
      - noTap
      - singleTap
      - doubleTap
      - longPress
    - print button press times to serial
*/

#include "PushButtonTapsAndPress.h"

const int BUTTON_PIN = 17;
//PushButtonTapsAndPress(int buttonPin, bool activeLow, bool serialPrintTapPressTimes)
PushButtonTapsAndPress pushBtn(BUTTON_PIN, true, true);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // noting 10 taps with tap press times
  int taps = 0;
  while(taps < 10) {
    if(pushBtn.checkButtonStatus() != ButtonTapType::noTap)
      taps++;
    delay(1);
  }

  // turning library tap press times print function off
  pushBtn.pushButtonSetup(BUTTON_PIN, true, false);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  switch(pushBtn.checkButtonStatus()) {
  case ButtonTapType::noTap:
    break;
  case ButtonTapType::singleTap:
    Serial.println("SINGLE TAP");
    break;
  case ButtonTapType::doubleTap:
    Serial.println("DOUBLE TAP");
    break;
  case ButtonTapType::longPress:
    Serial.println("LONG PRESS");
    break;
  default:
    Serial.println("switch case default -> code should never come here!");
    break;
  }
  delay(1);
}
