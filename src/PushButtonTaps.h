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
    - singleTap -> 250 milliseconds from button deactivation to give user time for doubleTap
    - doubleTap -> as soon as second tap occurs
    - longPress -> as soon as button is pressed for 600 milliseconds
  - How to use:
  - put the desired function inside loop() function, as shown in example
  - get classified button tap byte using checkButtonStatus()
  - get button active state using buttonActiveDebounced()
  - get last button press times use getLastTapTimes(bool &dataReady, uint16_t
    &firstTapMs, uint16_t &gapBetweenTapsMs, uint16_t &secondTapMs)
*/

#ifndef PUSH_BUTTON_TAPS_H
#define PUSH_BUTTON_TAPS_H

#include <Arduino.h>

class PushButtonTaps {
  public: 
    /* To note if current tap is single tap or double tap, library will wait 
    MAX_TIME_GAP_IN_DOUBLE_TAP_MS milliseconds to declare a tap as Single Tap
    or Double Tap. This will not be a program blocking wait. */
    const uint8_t MAX_TIME_GAP_IN_DOUBLE_TAP_MS = 250;
    /* Minimum time user will need to press and hold button
    to classify it as a long press */
    const uint16_t MIN_LONG_PRESS_TIME_MS = 600;
    /* Time between reads to debounce user-input */
    const uint16_t DEBOUNCE_TIME_US = 20;

    /* Constructor */
    PushButtonTaps();
    /* Constructor with button pin initialization */
    PushButtonTaps(uint8_t buttonPin);
    /* Function to initialize button pin */
    void setButtonPin(uint8_t buttonPin);
    /* Function to set button as Active Low or High. Default Active Low true */
    void setButtonActiveLow(bool activeLow);
    /* Get debounced active status of button. Includes DEBOUNCE_TIME_US microsecond for debounce */
    bool buttonActiveDebounced();
    /* Get button tap status in an 8 bit unsigned integer as output. Includes DEBOUNCE_TIME_US microsecond for debounce
    - classifies taps as one of 4 types
      - 0 -> noTap
      - 1 -> singleTap
      - 2 -> doubleTap
      - 3 -> longPress
    */
    byte checkButtonStatus();
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
    byte _currentTapIntermediateValue = 0;
    uint16_t _firstTapMs;
    uint16_t _gapBetweenTapsMs;
    uint16_t _secondTapMs;
};

#endif
