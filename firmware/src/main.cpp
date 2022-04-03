#include <Arduino.h>
#include "LedManager.h"
#include "MoistureManager.h"

#define MILLIS_USE_TIMERB0 // Use timer B for millis - we use timer A for PWM and the leds

volatile byte ledState = 0;

void setup() {
  takeOverTCA0();
  TCA0.SPLIT.CTRLD = 0x01; // Enable split mode  
  
  LedManager::setup();
  MoistureManager::setup();

  TCA0.SPLIT.CTRLA = TCA_SPLIT_ENABLE_bm | TCA_SPLIT_CLKSEL_DIV64_gc; //enable the timer 64 prescaler

  PORTA.PIN2CTRL |= (PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc); // Enable pin change interrupt and internal pullup
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

  if (ledState) {
    LedManager::turnOnLed(2);
  } else {
    LedManager::turnOffLed(2);
  }
}

ISR(PORTA_PORT_vect) {
  byte flags = PORTA.INTFLAGS;
  PORTA.INTFLAGS = flags; // Clear the int flags
  ledState = !ledState;
}