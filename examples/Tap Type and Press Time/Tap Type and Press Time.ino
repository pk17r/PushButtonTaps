/*
  Prashant Kumar
  Push Button Taps And Press
  https://github.com/pk17r/PushButtonTapsAndPress

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
*/

#include "PushButtonTapsAndPress.h"

const int BUTTON_PIN = 17;
PushButtonTapsAndPress pushBtn;

int tapsRecorded = 0;
int tapTimesRead = 0;
uint16_t firstTapMs;
uint16_t gapBetweenTapsMs;
uint16_t secondTapMs;

void setup() {
  Serial.begin(115200);
  while(!Serial) {
    delay(10);
  }
  delay(500);

  // button pin can be initialized here or directly at pushBtn decleration
  pushBtn.setButtonPin(BUTTON_PIN);

  Serial.println("TAP TYPE   First : Gap : Second");
}

void loop() {
  // check for button tap
  byte tap = pushBtn.checkButtonStatus();
  if(tap != 0) {
    switch(tap) {
      case 1:
        Serial.print("SINGLE TAP ");
        break;
      case 2:
        Serial.print("DOUBLE TAP ");
        break;
      case 3:
        Serial.print("LONG PRESS ");
        break;
    }
    tapsRecorded++;
  }
  // get last tap times
  bool dataReady = false;
  if(tapTimesRead < tapsRecorded) {
    pushBtn.getLastTapTimes(dataReady, firstTapMs, gapBetweenTapsMs, secondTapMs);
    if(dataReady) {
      tapTimesRead++;
      Serial.print(firstTapMs);
      Serial.print("ms : ");
      Serial.print(gapBetweenTapsMs);
      Serial.print("ms : ");
      Serial.print(secondTapMs);
      Serial.println("ms");
    }
  }
  delay(1);
}
