#include <Arduino.h>
#include "LedManager.h"
#include "MoistureManager.h"
#include "SleepManager.h"
#include "ConfigManager.h"
#include <avr/sleep.h>
#include "Constants.h"

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

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  startTime = millis();
}

void loop() {
  byte buttonIsPressed = digitalRead(BUTTON_PIN) == LOW;

  if ((millis()-startTime) > KEEP_AWAKE_TIME_MS && !buttonIsPressed) {
    SleepManager::sleep();
  }

  if (buttonIsPressed) {
    if (buttonPressTime == 0) {
      buttonPressTime = millis();
    } else if ((millis() - buttonPressTime) > WATER_POINT_SETTING_WAIT_MS) {
      ConfigManager::setWaterPoint();
      startTime = millis();
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