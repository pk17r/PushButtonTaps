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

#ifndef PUSH_BUTTON_TAPS_AND_PRESS_H
#define PUSH_BUTTON_TAPS_AND_PRESS_H

#include <Arduino.h>

// types of button presses
enum class ButtonTapType {  
  noTap, 
  singleTap, 
  doubleTap, 
  longPress
};

class PushButtonTapsAndPress {
  public: 
    /* To note if current tap is single tap or double tap, library will wait 
    MAX_TIME_GAP_IN_DOUBLE_TAP_MS milliseconds to declare a tap as Single Tap
    or Double Tap. This will not be a program blocking wait. */
    const uint8_t MAX_TIME_GAP_IN_DOUBLE_TAP_MS = 250;
    /* Minimum time user will need to press and hold button
    to classify it as a long press */
    const uint16_t MIN_LONG_PRESS_TIME_MS = 650;

    /* Constructor */
    PushButtonTapsAndPress();
    /* Constructor with button pin initialization */
    PushButtonTapsAndPress(uint8_t buttonPin);
    /* Function to set button as Active Low or High. Default Active Low true */
    void setButtonActiveLow(bool activeLow);
    /* Get debounced active status of button. Includes 1 microsecond for debounce */
    bool buttonActiveDebounced();
    /* Get button tap status in ButtonTapType enum class variable as output. Includes 1 microsecond for debounce */
    ButtonTapType checkButtonStatus();
    /* Get tap times once Button Pin turns Off */
    void getLastTapTimes(bool &dataReady, uint16_t &firstTapMs, uint16_t &gapBetweenTapsMs, uint16_t &secondTapMs);

  private: 
    /* GPIO on which Push Button is mounted */
    uint8_t _BUTTON_PIN = -1;
    /* Set button as Active Low or Active High */
    bool _activeLow = true;
    
    // variables to track button press times
    unsigned long _lastButtonPressStartTimeMs;
    unsigned long _lastButtonPressEndTimeMs;
    bool _lastStateActive = false;
    ButtonTapType _currentTapIntermediateValue = ButtonTapType::noTap;
    uint16_t _firstTapMs;
    uint16_t _gapBetweenTapsMs;
    uint16_t _secondTapMs;
};

#endif
