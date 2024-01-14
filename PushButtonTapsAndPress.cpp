/*
  Author: Prashant Kumar

  Push Button Taps And Press
    - Arduino library to read Debounced Push Button Output as Single Tap, Double
      Tap or Long Press without using while statements or blocking program flow
    - classifies taps as one of 4 types (enum class ButtonTapType)
      - noTap
      - singleTap
      - doubleTap
      - longPress
    - option to set button Active Low or High. Default Active Low
    - How to use: check button status continously using checkButtonStatus() to
      get tap type or using buttonActiveDebounced() to get active state
    - To get last button press times use getLastTapTimes
*/

#include "PushButtonTapsAndPress.h"
#define _serialPrintButtonPressTimes 0

/* Constructor */
PushButtonTapsAndPress::PushButtonTapsAndPress() {
}

/* Constructor with button pin initialization */
PushButtonTapsAndPress::PushButtonTapsAndPress(uint8_t buttonPin) {
  //initialize button
  _BUTTON_PIN = buttonPin;
  pinMode(_BUTTON_PIN, INPUT);
}

/* Function to set button as Active Low or High. Default Active Low true */
void PushButtonTapsAndPress::setButtonActiveLow(bool activeLow) {
  _activeLow = activeLow;
}

/* Get debounced active status of button. Includes 1 microsecond for debounce */
bool PushButtonTapsAndPress::buttonActiveDebounced() {
  uint8_t reading1 = digitalRead(_BUTTON_PIN);
  delayMicroseconds(1);
  uint8_t reading2 = digitalRead(_BUTTON_PIN);
  while(reading2 != reading1) {
    reading1 = digitalRead(_BUTTON_PIN);
    delayMicroseconds(1);
    reading2 = digitalRead(_BUTTON_PIN);
  }
  if(_activeLow)
    return !reading2;  //active low
  else
    return reading2;  //active high
}

/* Get button tap status in ButtonTapType enum class variable as output. Includes 1 microsecond for debounce */
ButtonTapType PushButtonTapsAndPress::checkButtonStatus() {
  /*  Assumption: Microcontroller checks Button Status every 50 milliseconds or faster
      Cases:
        1. No Tap
            buttonActiveDebounced() is LOW   &&   _currentTapIntermediateValue == 0
        2. Single Tap
            buttonActiveDebounced() is LOW   &&   _currentTapIntermediateValue == 1   &&   (millis() - _lastButtonPressEndTimeMs) > MAX_TIME_GAP_IN_DOUBLE_TAP_MS
        3. Double Tap
            buttonActiveDebounced() is HIGH   &&   _currentTapIntermediateValue == 1
        4. Long Tap
            (_currentTapIntermediateValue == 1   &&   (millis() - _lastButtonPressStartTimeMs) > MIN_LONG_PRESS_TIME_MS)
  */
  ButtonTapType _currentButtonTap = ButtonTapType::noTap;

  if(buttonActiveDebounced() && !_lastStateActive) {
    // push button went from low to high
    // can be a double tap

    // if single tap has already happened and this tap is before MAX_TIME_GAP_IN_DOUBLE_TAP_MS
    if(_currentTapIntermediateValue == ButtonTapType::singleTap) {
      // it is a double tap!
      _currentButtonTap = ButtonTapType::doubleTap;
      _currentTapIntermediateValue = ButtonTapType::doubleTap;
      _firstTapMs = _lastButtonPressEndTimeMs - _lastButtonPressStartTimeMs;
      _lastButtonPressStartTimeMs = millis();
      _gapBetweenTapsMs = _lastButtonPressStartTimeMs - _lastButtonPressEndTimeMs;
      _lastStateActive = true;

      if(_serialPrintButtonPressTimes) {
        Serial.print("DOUBLE TAP ");
        // first tap time
        Serial.print(_firstTapMs);
        Serial.print("ms : ");
        // gap between first and second tap
        Serial.print(_gapBetweenTapsMs);
        Serial.print("ms");
      }
    }
    // if this is the first tap
    else if(_currentTapIntermediateValue == ButtonTapType::noTap) {
      _currentButtonTap = ButtonTapType::noTap;
      _currentTapIntermediateValue = ButtonTapType::singleTap;
      _lastButtonPressStartTimeMs = millis();
      _lastStateActive = true;
    }

  }
  else if(buttonActiveDebounced() && _lastStateActive) {
    // push button continues to be in high state
    // can be a long press

    if(_currentTapIntermediateValue == ButtonTapType::singleTap && (millis() - _lastButtonPressStartTimeMs >= MIN_LONG_PRESS_TIME_MS)) {
      // it is a long press!
      _currentButtonTap = ButtonTapType::longPress;
      _currentTapIntermediateValue = ButtonTapType::longPress;
      if(_serialPrintButtonPressTimes) {
        Serial.print("LONG PRESS ");
      }
    }

  }
  else if(!buttonActiveDebounced() && _lastStateActive) {
    // push button went from high to low
    _lastButtonPressEndTimeMs = millis();
    _lastStateActive = false;
    // reset double tap
    if(_currentTapIntermediateValue == ButtonTapType::doubleTap) {
      _currentTapIntermediateValue = ButtonTapType::noTap;
      _secondTapMs = _lastButtonPressEndTimeMs - _lastButtonPressStartTimeMs;
      if(_serialPrintButtonPressTimes) {
        // second tap time
        Serial.print(" : ");
        Serial.print(_secondTapMs);
        Serial.println("ms");
      }
    }
    // reset long press
    else if(_currentTapIntermediateValue == ButtonTapType::longPress) {
      _currentTapIntermediateValue = ButtonTapType::noTap;
      _firstTapMs = _lastButtonPressEndTimeMs - _lastButtonPressStartTimeMs;
      _gapBetweenTapsMs = 0;
      _secondTapMs = 0;
      if(_serialPrintButtonPressTimes) {
        // long press time
        Serial.print(_firstTapMs);
        Serial.println("ms");
      }
    }
    
  }
  else if(!buttonActiveDebounced() && !_lastStateActive) {
    // push button continues to be in low state
    // last tap can be a single tap

    if(_currentTapIntermediateValue == ButtonTapType::singleTap && (millis() - _lastButtonPressEndTimeMs >= MAX_TIME_GAP_IN_DOUBLE_TAP_MS)) {
      // it is a single tap!
      _currentButtonTap = ButtonTapType::singleTap;
      _currentTapIntermediateValue = ButtonTapType::noTap;
      _firstTapMs = _lastButtonPressEndTimeMs - _lastButtonPressStartTimeMs;
      _gapBetweenTapsMs = 0;
      _secondTapMs = 0;
      if(_serialPrintButtonPressTimes) {
        Serial.print("SINGLE TAP ");
        Serial.print(_firstTapMs);
        Serial.println("ms");
      }
    }
    else if(_currentTapIntermediateValue == ButtonTapType::noTap) {
      // there continues to be no tap
      _currentButtonTap = ButtonTapType::noTap;
    }
    
  }
  return _currentButtonTap;
}

/* Get tap times once Button Pin turns Off */
void PushButtonTapsAndPress::getLastTapTimes(bool &dataReady, uint16_t &firstTapMs, uint16_t &gapBetweenTapsMs, uint16_t &secondTapMs) {
  dataReady = _currentTapIntermediateValue == ButtonTapType::noTap;
  firstTapMs = _firstTapMs;
  gapBetweenTapsMs = _gapBetweenTapsMs;
  secondTapMs = _secondTapMs;
}
