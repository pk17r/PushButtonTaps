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

// types of button presses
enum class ButtonTapType {  
  noTap, 
  singleTap, 
  doubleTap, 
  longPress
};

#ifndef PUSH_BUTTON_TAPS_AND_PRESS_H
#define PUSH_BUTTON_TAPS_AND_PRESS_H

#include <Arduino.h>

class PushButtonTapsAndPress {
  public: 
    const unsigned long MAX_TIME_GAP_IN_DOUBLE_TAP_MS = 250;
    const unsigned long MIN_LONG_PRESS_TIME_MS = 650;

    PushButtonTapsAndPress();
    PushButtonTapsAndPress(int buttonPin, bool activeLow, bool serialPrintTapPressTimes);
    void pushButtonSetup(int buttonPin, bool activeLow, bool serialPrintTapPressTimes);
    ButtonTapType checkButtonStatus();
	bool buttonActive();

  private: 
    int _BUTTON_PIN = -1;
    bool _serialPrintTapPressTimes = false;
    bool _activeLow = true;
    
    // last button tap details
    unsigned long _lastButtonPressStartTimeMs;
    unsigned long _lastButtonPressEndTimeMs;
    bool _lastStateActive = false;
    int _lastConsecitiveTaps = 0;
};

#endif