# Push Button Taps

Arduino library to read Debounced Push Button Output as Single Tap, Double
Tap or Long Press without blocking program flow


## Description

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