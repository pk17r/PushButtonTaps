# Push Button Taps And Press

Arduino library to read Debounced Push Button Output as Single Tap, Double
Tap or Long Press without using while statements or blocking program flow


## Description

- Arduino library to read Debounced Push Button Output as Single Tap, Double
  Tap or Long Press without using while statements or blocking program flow
- classifies taps as one of 4 types
  - 0 -> noTap
  - 1 -> singleTap
  - 2 -> doubleTap
  - 3 -> longPress
- option to set button Active Low or High. Default Active Low
- To classify the tap, there is some delay from user input to tap classification.
- Delays: 
  - noTap -> 1 microsecond to debounce
  - singleTap -> 250 milliseconds from button deactivation to give user time for doubleTap
  - doubleTap -> as soon as second tap occurs
  - longPress -> as soon as button is pressed for 650 milliseconds
- How to use:
- get classified button tap byte using checkButtonStatus()
- get button active state using buttonActiveDebounced()
- get last button press times use getLastTapTimes(bool &dataReady, uint16_t
  &firstTapMs, uint16_t &gapBetweenTapsMs, uint16_t &secondTapMs)