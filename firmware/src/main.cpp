#include <Arduino.h>
#include "LedManager.h"
#include "MoistureManager.h"
#include "SleepManager.h"

#ifndef MILLIS_USE_TIMERB1
  #error "This sketch is written for use with TCB1 as the millis timing source"
#endif

void setup() {
  takeOverTCA0();
  TCA0.SPLIT.CTRLD = 0x01; // Enable split mode  
  
  LedManager::setup();
  MoistureManager::setup();

  TCA0.SPLIT.CTRLA = TCA_SPLIT_ENABLE_bm | TCA_SPLIT_CLKSEL_DIV64_gc; //enable the timer 64 prescaler
  delay(100);
}

void loop() {
  if (millis() > 1000) {
    SleepManager::sleep();
  }


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