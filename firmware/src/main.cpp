#include <Arduino.h>
#include "LedManager.h"
#include "MoistureManager.h"
#include "SleepManager.h"
#include "ConfigManager.h"
#include <avr/sleep.h>

#ifndef MILLIS_USE_TIMERB1
  #error "This sketch is written for use with TCB1 as the millis timing source"
#endif

long startTime = 0;

long buttonPressTime = 0;

void setup() {
  takeOverTCA0();
  TCA0.SPLIT.CTRLD = 0x01; // Enable split mode  
  
  LedManager::setup();
  MoistureManager::setup();
  SleepManager::setup();

  TCA0.SPLIT.CTRLA = TCA_SPLIT_ENABLE_bm | TCA_SPLIT_CLKSEL_DIV64_gc; //enable the timer 64 prescaler
  delay(100);
  ConfigManager::loadConfig();

  delay(250); // Wait for moisture reading to stabilise

  pinMode(PIN_PA2, INPUT_PULLUP);

  startTime = millis();
}

void loop() {
  if ((millis()-startTime) > 3000 && buttonPressTime == 0) {
    SleepManager::sleep();
  }

  if (digitalRead(PIN_PA2) == LOW) {
    if (buttonPressTime == 0) {
      buttonPressTime = millis();
    } else if ((millis() - buttonPressTime) > 1000*10) {
      ConfigManager::setWaterPoint();
      buttonPressTime = 0;
    }
  } else {
    buttonPressTime = 0;
  }

  int moisture = MoistureManager::getNormalisedReading();
  for (int i = 0; i < 12; i++) {
    if ((12-i) == ConfigManager::NORMALISED_WATER_POINT) {
      LedManager::flashLed(i, LedManager::FlashRate::SLOW);
    } else if ((12-i) <= moisture) {
      LedManager::turnOnLed(i);
    } else {
      LedManager::turnOffLed(i);
    }
  }
  delay(100);
}