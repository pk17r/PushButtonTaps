/*
  Prashant Kumar
  Push Button Taps
  https://github.com/pk17r/PushButtonTaps

  - Arduino library to read Debounced Push Button Output as Single Tap, Double
    Tap or Long Press without blocking program flow
  - classifies taps as one of 4 types
    - 0 -> noTap
    - 1 -> singleTap
    - 2 -> doubleTap
    - 3 -> longPress
  - option to set button Active Low or High. Default Active Low
  - To classify the tap, there is some delay from user input to tap classification.
  - Delays in Read from User Input:
    - noTap -> 1 microsecond to debounce
    - singleTap -> 150 milliseconds from button deactivation to give user time for doubleTap
    - doubleTap -> as soon as second tap occurs
    - longPress -> as soon as button is pressed for 500 milliseconds
  - How to use:
  - put the desired function inside loop() function, as shown in example
  - get classified button tap byte using checkButtonStatus()
  - get button active state using buttonActiveDebounced()
  - get last button press times use getLastTapTimes(bool &dataReady, uint16_t
    &firstTapMs, uint16_t &gapBetweenTapsMs, uint16_t &secondTapMs)
*/

#include "PushButtonTaps.h"
#define _serialPrintButtonPressTimes 0

/* Constructor */
PushButtonTaps::PushButtonTaps() {
}

/* Constructor with button pin initialization */
PushButtonTaps::PushButtonTaps(uint8_t buttonPin) {
  setButtonPin(buttonPin);
}

/* Function to initialize button pin */
void PushButtonTaps::setButtonPin(uint8_t buttonPin) {
  //initialize button
  _BUTTON_PIN = buttonPin;
  pinMode(_BUTTON_PIN, INPUT);
}

/* Function to set button as Active Low or High. Default Active Low true */
void PushButtonTaps::setButtonActiveLow(bool activeLow) {
  _activeLow = activeLow;
}

/* Get debounced active status of button. Includes 1 microsecond for debounce */
bool PushButtonTaps::buttonActiveDebounced() {
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

/* Get button tap status in an 8 bit unsigned integer as output. Includes 1 microsecond for debounce */
byte PushButtonTaps::checkButtonStatus() {
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
  byte _currentButtonTap = 0;

  if(buttonActiveDebounced() && !_lastStateActive) {
    // push button went from low to high
    // can be a double tap

    // if single tap has already happened and this tap is before MAX_TIME_GAP_IN_DOUBLE_TAP_MS
    if(_currentTapIntermediateValue == 1) {
      // it is a double tap!
      _currentButtonTap = 2;
      _currentTapIntermediateValue = 2;
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
    else if(_currentTapIntermediateValue == 0) {
      _currentButtonTap = 0;
      _currentTapIntermediateValue = 1;
      _lastButtonPressStartTimeMs = millis();
      _lastStateActive = true;
    }

  }
  else if(buttonActiveDebounced() && _lastStateActive) {
    // push button continues to be in high state
    // can be a long press

    if(_currentTapIntermediateValue == 1 && (millis() - _lastButtonPressStartTimeMs >= MIN_LONG_PRESS_TIME_MS)) {
      // it is a long press!
      _currentButtonTap = 3;
      _currentTapIntermediateValue = 3;
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
    if(_currentTapIntermediateValue == 2) {
      _currentTapIntermediateValue = 0;
      _secondTapMs = _lastButtonPressEndTimeMs - _lastButtonPressStartTimeMs;
      if(_serialPrintButtonPressTimes) {
        // second tap time
        Serial.print(" : ");
        Serial.print(_secondTapMs);
        Serial.println("ms");
      }
    }
    // reset long press
    else if(_currentTapIntermediateValue == 3) {
      _currentTapIntermediateValue = 0;
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

    if(_currentTapIntermediateValue == 1 && (millis() - _lastButtonPressEndTimeMs >= MAX_TIME_GAP_IN_DOUBLE_TAP_MS)) {
      // it is a single tap!
      _currentButtonTap = 1;
      _currentTapIntermediateValue = 0;
      _firstTapMs = _lastButtonPressEndTimeMs - _lastButtonPressStartTimeMs;
      _gapBetweenTapsMs = 0;
      _secondTapMs = 0;
      if(_serialPrintButtonPressTimes) {
        Serial.print("SINGLE TAP ");
        Serial.print(_firstTapMs);
        Serial.println("ms");
      }
    }
    else if(_currentTapIntermediateValue == 0) {
      // there continues to be no tap
      _currentButtonTap = 0;
    }
    
  }
  return _currentButtonTap;
}

/* Get tap times once Button Pin turns Off */
void PushButtonTaps::getLastTapTimes(bool &dataReady, uint16_t &firstTapMs, uint16_t &gapBetweenTapsMs, uint16_t &secondTapMs) {
  dataReady = _currentTapIntermediateValue == 0;
  firstTapMs = _firstTapMs;
  gapBetweenTapsMs = _gapBetweenTapsMs;
  secondTapMs = _secondTapMs;
}
