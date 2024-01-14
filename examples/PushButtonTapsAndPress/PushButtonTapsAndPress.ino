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

const int BUTTON_PIN = 17;
PushButtonTapsAndPress pushBtn;

int tapsRecorded = 0;
int tapTimesRead = 0;
uint16_t firstTapMs;
uint16_t gapBetweenTapsMs;
uint16_t secondTapMs;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pushBtn.setButtonPin(BUTTON_PIN);

  while(!Serial) {
    delay(10);
  }
  Serial.println("TAP TYPE   First : Gap : Second");
}

void loop() {
  // check for button tap
  ButtonTapType tap = pushBtn.checkButtonStatus();
  if(tap != ButtonTapType::noTap) {
    switch(tap) {
      case ButtonTapType::singleTap:
        Serial.print("SINGLE TAP ");
        break;
      case ButtonTapType::doubleTap:
        Serial.print("DOUBLE TAP ");
        break;
      case ButtonTapType::longPress:
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
