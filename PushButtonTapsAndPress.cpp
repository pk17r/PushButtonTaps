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

PushButtonTapsAndPress::PushButtonTapsAndPress() {
}

PushButtonTapsAndPress::PushButtonTapsAndPress(int buttonPin, bool activeLow, bool serialPrintTapPressTimes) {
  pushButtonSetup(buttonPin, activeLow, serialPrintTapPressTimes);
}

void PushButtonTapsAndPress::pushButtonSetup(int buttonPin, bool activeLow, bool serialPrintTapPressTimes) {
  //initialize button
  _BUTTON_PIN = buttonPin;
  pinMode(_BUTTON_PIN, INPUT);
  _activeLow = activeLow;
  _serialPrintTapPressTimes = serialPrintTapPressTimes;
  if(_serialPrintTapPressTimes) {
    Serial.print("Push Button on gpio ");
    Serial.print(_BUTTON_PIN);
    Serial.println("setup!");
  }
}

bool PushButtonTapsAndPress::_buttonActive() {
  if(_activeLow)
    return !digitalRead(_BUTTON_PIN);  //active low
  else
    return !digitalRead(_BUTTON_PIN);  //active high
}

ButtonTapType PushButtonTapsAndPress::checkButtonStatus() {
  /*  Assumption: Microcontroller checks Button Status every 50 milliseconds or faster
      Cases:
        1. No Tap
            _buttonActive() is LOW   &&   _lastConsecitiveTaps == 0
        2. Single Tap
            _buttonActive() is LOW   &&   _lastConsecitiveTaps == 1   &&   (millis() - _lastButtonPressEndTimeMs) > MAX_TIME_GAP_IN_DOUBLE_TAP_MS
        3. Double Tap
            _buttonActive() is HIGH   &&   _lastConsecitiveTaps == 1
        4. Long Tap
            (_lastConsecitiveTaps == 1   &&   (millis() - _lastButtonPressStartTimeMs) > MIN_LONG_PRESS_TIME_MS)
  */
  ButtonTapType _currentButtonTap = ButtonTapType::noTap;

  if(_buttonActive() && !_lastStateActive) {
    // push button went from low to high
    // can be a double tap

    // if single tap has already happened and this tap is before MAX_TIME_GAP_IN_DOUBLE_TAP_MS
    if(_lastConsecitiveTaps == 1) {
      // it is a double tap!
      _currentButtonTap = ButtonTapType::doubleTap;
      _lastConsecitiveTaps = 2;
      unsigned long _firstTapPressTime = _lastButtonPressEndTimeMs - _lastButtonPressStartTimeMs;
      _lastButtonPressStartTimeMs = millis();
      _lastStateActive = true;

      if(_serialPrintTapPressTimes) {
        Serial.print("DOUBLE TAP ");
        // first tap time
        Serial.print(_firstTapPressTime);
        Serial.print("ms : ");
        // gap between first and second tap
        Serial.print(_lastButtonPressStartTimeMs - _lastButtonPressEndTimeMs);
        Serial.print("ms");
      }
    }
    // if this is the first tap
    else if(_lastConsecitiveTaps == 0) {
      _currentButtonTap = ButtonTapType::noTap;
      _lastConsecitiveTaps = 1;
      _lastButtonPressStartTimeMs = millis();
      _lastStateActive = true;
    }

  }
  else if(_buttonActive() && _lastStateActive) {
    // push button continues to be in high state
    // can be a long press

    if(_lastConsecitiveTaps == 1 && (millis() - _lastButtonPressStartTimeMs >= MIN_LONG_PRESS_TIME_MS)) {
      // it is a long press!
      _currentButtonTap = ButtonTapType::longPress;
      _lastConsecitiveTaps = -1;
      if(_serialPrintTapPressTimes) {
        Serial.print("LONG PRESS ");
      }
    }

  }
  else if(!_buttonActive() && _lastStateActive) {
    // push button went from high to low
    _lastButtonPressEndTimeMs = millis();
    _lastStateActive = false;
    // reset double tap
    if(_lastConsecitiveTaps == 2) {
      _lastConsecitiveTaps = 0;
      if(_serialPrintTapPressTimes) {
        // second tap time
        Serial.print(" : ");
        Serial.print(_lastButtonPressEndTimeMs - _lastButtonPressStartTimeMs);
        Serial.println("ms");
      }
    }
    // reset long press
    else if(_lastConsecitiveTaps == -1) {
      _lastConsecitiveTaps = 0;
      if(_serialPrintTapPressTimes) {
        // long press time
        Serial.print(_lastButtonPressEndTimeMs - _lastButtonPressStartTimeMs);
        Serial.println("ms");
      }
    }
    
  }
  else if(!_buttonActive() && !_lastStateActive) {
    // push button continues to be in low state
    // last tap can be a single tap

    if(_lastConsecitiveTaps == 1 && (millis() - _lastButtonPressEndTimeMs >= MAX_TIME_GAP_IN_DOUBLE_TAP_MS)) {
      // it is a single tap!
      _currentButtonTap = ButtonTapType::singleTap;
      _lastConsecitiveTaps = 0;
      if(_serialPrintTapPressTimes) {
        Serial.print("SINGLE TAP ");
        Serial.print(_lastButtonPressEndTimeMs - _lastButtonPressStartTimeMs);
        Serial.println("ms");
      }
    }
    else if(_lastConsecitiveTaps == 0) {
      // there continues to be no tap
      _currentButtonTap = ButtonTapType::noTap;
    }
    
  }
  return _currentButtonTap;
}
