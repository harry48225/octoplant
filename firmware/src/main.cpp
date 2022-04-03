#include <Arduino.h>
#include "LedManager.h"
#include "MoistureManager.h"

#define MILLIS_USE_TIMERB0 // Use timer B for millis - we use timer A for PWM and the leds

void setup() {
  takeOverTCA0();
  TCA0.SPLIT.CTRLD = 0x01; // Enable split mode  
  
  LedManager::setup();
  MoistureManager::setup();

  TCA0.SPLIT.CTRLA = TCA_SPLIT_ENABLE_bm | TCA_SPLIT_CLKSEL_DIV64_gc; //enable the timer 64 prescaler
}

void loop() {
  int moisture = MoistureManager::getNormalisedReading();
  for (int i = 0; i < 12; i++) {
    if ((12-i) <= moisture) {
      LedManager::turnOnLed(i);
    }
    else {
      LedManager::turnOffLed(i);
    }
  }
}