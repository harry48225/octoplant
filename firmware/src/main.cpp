#include <Arduino.h>
#include "LedManager.h"

#define MILLIS_USE_TIMERB0 // Use timer B for millis - we use timer A for PWM and the leds

int sensorOutput = PIN_PA1;

void setup() {
  analogReadResolution(12);
  pinMode(sensorOutput, INPUT);

  takeOverTCA0();
  TCA0.SPLIT.CTRLD = 0x01; // Enable split mode  
  
  pinMode(PIN_PA3, OUTPUT);

  TCA0.SPLIT.CTRLB = TCA_SPLIT_HCMP0EN_bm; // PWM on WO3 - PA3
  TCA0.SPLIT.HPER = 0x02; // Count all the way down from 0x04 (4) At 10MHz, this gives ((10MHz / 64)/4) ~40KHz PWM
  TCA0.SPLIT.HCMP0  = 0x01; // 50% duty cycle
  
  LedManager::setup();

  TCA0.SPLIT.CTRLA = TCA_SPLIT_ENABLE_bm | TCA_SPLIT_CLKSEL_DIV64_gc; //enable the timer 64 prescaler
}

int DRYPOINT = 3200;
int WETPOINT = 1500;

void loop() {
  int moisture = analogRead(sensorOutput);
  moisture -= WETPOINT;
  moisture /= ((DRYPOINT - WETPOINT)/12);
  moisture = 12 - constrain(moisture, 0, 12);
  for (int i = 0; i < 12; i++) {
    if ((12-i) <= moisture) {
      LedManager::turnOnLed(i);
    }
    else {
      LedManager::turnOffLed(i);
    }
  }
}