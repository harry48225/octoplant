#include "LedManager.h"
#include <Arduino.h>

namespace LedManager {
  /*  Manages the charlieplexed leds */
  int _pins[4] = {PIN_PA7, PIN_PA6, PIN_PA5, PIN_PA4};
  int _ledState[12] = {0,0,0,0,0,0,0,0,0,0,0,0}; // This might be better as a 16 bit number
  int _currentLed = 0;

  void _openCurrentPath(int from, int to) {
    pinMode(_pins[from], OUTPUT);
    pinMode(_pins[to], OUTPUT);
    digitalWrite(_pins[from], HIGH);
    digitalWrite(_pins[to], LOW);
  }

  void _closeCurrentPaths() {
    pinMode(_pins[0], INPUT);  
    pinMode(_pins[1], INPUT);  
    pinMode(_pins[2], INPUT);  
    pinMode(_pins[3], INPUT);
  }

  void turnOnLed(int ledNumber) {
    _ledState[ledNumber] = 1;
  }

  void turnOffLed(int ledNumber) {
    _ledState[ledNumber] = 0;
  }

  void _createCurrentPathForLed(int ledNumber) {
    switch (ledNumber) {
      case 0:
        _openCurrentPath(0,1);
        break; 
      case 6:
        _openCurrentPath(0,2);
        break;
      case 10:
        _openCurrentPath(0,3);
        break; 
      case 1:
        _openCurrentPath(1,0);
        break;
      case 2:
        _openCurrentPath(1,2);
        break; 
      case 8:
        _openCurrentPath(1,3);
        break;
      case 7:
        _openCurrentPath(2,0);
        break; 
      case 3:
        _openCurrentPath(2,1);
        break;
      case 4:
        _openCurrentPath(2,3);
        break; 
      case 11:
        _openCurrentPath(3,0);
        break;
      case 9:
        _openCurrentPath(3,1);
        break; 
      case 5:
        _openCurrentPath(3,2);
        break;    
  }
  }

  void setup() {
    // There is a 64 prescaler on TCA0
    TCA0.SPLIT.LPER = 0xFF; // Count down from 0xFF (255), At 10MHz  (10MHz/255)/64 ~ 600Hz
    TCA0.SPLIT.LCMP0 = 0x0F; // Trigger interrupt after half count
    TCA0.SPLIT.INTCTRL |= 0x10; // Enable interrupt on LCMP0
  }

  // Triggers ~600Hz
  ISR(TCA0_LCMP0_vect) {
    _closeCurrentPaths();
    if (_ledState[_currentLed] == 1) {
      _createCurrentPathForLed(_currentLed);
    }
    _currentLed = (_currentLed + 1) % 12;
    TCA0.SPLIT.INTFLAGS = 0xFF; // Clear the int flags otherwise millis breaks
  }
}

