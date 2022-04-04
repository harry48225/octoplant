#include "LedManager.h"
#include <Arduino.h>

namespace LedManager {
  /*  Manages the charlieplexed leds */
  int _pins[4] = {PIN_PA7, PIN_PA6, PIN_PA5, PIN_PA4};
  int _ledEnable[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
  // 255 is on, < 127 is off
  int _ledState[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
  // Gets subtracted from the led state each cycle
  int _flashModifier[12] = {8,8,8,8,16,16,4,4,8,8,8,8};
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
    if (_ledEnable[ledNumber] == 0) {
      _ledEnable[ledNumber] = 1;
      _ledState[ledNumber] = 255;
    }
  }

  void turnOffLed(int ledNumber) {
    if (_ledEnable[ledNumber] == 1) {
      _ledEnable[ledNumber] = 0;
      _ledState[ledNumber] = 0;
    }
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
    TCA0.SPLIT.LCMP0 = 0xFF; // Trigger interrupt after full count
    TCA0.SPLIT.INTCTRL |= 0x10; // Enable interrupt on LCMP0
  }

  // Triggers ~600Hz -- This is probably too high given how much computation happens in the ISR
  ISR(TCA0_LCMP0_vect) {
    TCA0.SPLIT.INTFLAGS = 0xFF; // Clear the int flags otherwise millis breaks
    _closeCurrentPaths();

    if (_ledEnable[_currentLed] == 1) {
      if (_ledState[_currentLed] < _flashModifier[_currentLed]) {
        _ledState[_currentLed] = 255 - (_flashModifier[_currentLed] - _ledState[_currentLed]);
      } else {
        _ledState[_currentLed] -= _flashModifier[_currentLed];
      }

      if (_ledState[_currentLed] > 127)
      {
        _createCurrentPathForLed(_currentLed);
      }
    }

    _currentLed = (_currentLed + 1) % 12; 
  }
}

