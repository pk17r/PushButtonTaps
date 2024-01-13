# Push Button Taps And Press

Arduino library to read Push Button as Single Tap, Double Tap or Long Press
without using while statements or stopping program flow

## Description

- setup single push button with a pull-up or pull-down 10K resistor 
  and a 100nF decoupling capacitor to remove button bounce.
- setup button as active high or low
- Example: Active Low schematic:
~~~{.cpp}
		  MCU Pin ----------------\
                  5V/3.3V ------[10K]------\
                                       /----\
				[100nF]      ]O[
                                       \----/
		  GND ---------------------/
~~~
- classifies taps as one of 4 types (enum class ButtonTapType)
  - noTap
  - singleTap
  - doubleTap
  - longPress
- print button press times to serial
