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
  - noTap -> 20 microsecond (us) to debounce  
  - singleTap -> 250 milliseconds (ms) from button deactivation to give user time for doubleTap  
  - doubleTap -> as soon as second tap occurs  
  - longPress -> as soon as button is pressed for 600 milliseconds (ms)  
- How to use:  
- put the desired function inside loop() function, as shown in example  
- get classified button tap byte using checkButtonStatus()  
- get button active state using buttonActiveDebounced()  
- get last button press times use getLastTapTimes(bool &dataReady, uint16_t  
  &firstTapMs, uint16_t &gapBetweenTapsMs, uint16_t &secondTapMs)  
  
  
## Usage  
  
- To set a pin as Input_Pull_up and Active Low, use the following:   
  
-- define push_button as a pointer:  
```
PushButtonTaps* push_button = new PushButtonTaps(BUTTON_PIN);   
```
-- or define push_button as an object:  
```
PushButtonTaps push_button(BUTTON_PIN);  
```
  
- note if button is pressed  
```
bool push_button_pressed = push_button->buttonActiveDebounced();  
```
or  
```
bool push_button_pressed = push_button.buttonActiveDebounced();  
```

- Get button tap status in an 8 bit unsigned integer as output. Includes DEBOUNCE_TIME_US microsecond for debounce  
- classifies taps as one of 4 types  
  - 0 -> noTap  
  - 1 -> singleTap  
  - 2 -> doubleTap  
  - 3 -> longPress  
```
byte btn_status = push_button->checkButtonStatus();  
```
or  
```
byte btn_status = push_button.checkButtonStatus();  
```

- Set button un-pressed state as active low (default) or active high:  
```
push_button->setButtonActiveLow(true or false);  
```
or  
```
push_button.setButtonActiveLow(true or false);  
```