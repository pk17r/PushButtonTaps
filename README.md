# Push Button Taps And Press

Arduino library to read Debounced Push Button Output as Single Tap, Double
Tap or Long Press without using while statements or blocking program flow


## Description

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