#include "ConfigManager.h"

#include <EEPROM.h>
#include <LedManager.h>
#include <Arduino.h>
#include "MoistureManager.h"
#include "Constants.h"

// Handles storing data in the EEPROM
// MAP
#define DRY_POINT_ADDR 0
#define WET_POINT_ADDR 2
#define NORMALISED_WATER_POINT_ADDR 4

namespace ConfigManager {
  int DRY_POINT;// = 3200;
  int WET_POINT;// = 1500;
  int NORMALISED_WATER_POINT;// = 3;

  void programSuccessfulAnimation() {
    LedManager::resetLeds();

    // Display programming animation
    for (int i = 0; i <= 6; i++) {
      LedManager::turnOnLed(6-i);
      LedManager::turnOnLed(5+i);
      delay(20);
    }

    LedManager::resetLeds();

    delay(200);
  }

  void waitForButtonRelease() {
    while (digitalRead(BUTTON_PIN) == LOW) {
      delay(10);
    }
  }

  void waitForButtonPress() {
    while (digitalRead(BUTTON_PIN) == HIGH) {
      delay(10);
    }
  }

  void setWaterPoint() {
    // Set the water point as the current normalised water reading
    // Should be triggered on long press
    NORMALISED_WATER_POINT = MoistureManager::getNormalisedReading();
    LedManager::flashLed(12-NORMALISED_WATER_POINT, LedManager::FlashRate::FAST);
    waitForButtonRelease();
    delay(200);
    writeConfig();
    programSuccessfulAnimation();
  }

  void calibrate() {
    // Enable button
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    
    // Set the wet point
    // Flash all leds
    for (int i = 0; i < 12; i++) {
      LedManager::flashLed(i, LedManager::FlashRate::FAST);
    }

    waitForButtonRelease();
    waitForButtonPress();
    WET_POINT = MoistureManager::getRawReading() + 100;
    LedManager::resetLeds();
    waitForButtonRelease();
    
    // Set the dry point  
    // Flash the lowest LED
    LedManager::flashLed(11, LedManager::FlashRate::FAST);
    waitForButtonPress();
    DRY_POINT = MoistureManager::getRawReading() - 100;
    waitForButtonRelease();
    writeConfig();

    programSuccessfulAnimation();
  }

  void loadConfig() {
    // First determine the reset cause

    byte isPowerOnReset = 0;
    // Megatinycore stores the reset flags in GPIO0 and automatically clears them
    byte reset_flags = GPIO0;
    // If it's not a software reset we may want to enter calibration mode
    if (reset_flags & RSTCTRL_PORF_bm) {
      isPowerOnReset = 1;
    }

    // Reads the configuration from EEPROM
    EEPROM.get(DRY_POINT_ADDR, DRY_POINT);
    EEPROM.get(WET_POINT_ADDR, WET_POINT);

    EEPROM.get(NORMALISED_WATER_POINT_ADDR, NORMALISED_WATER_POINT);

    // Enable button
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    // Calibrate if the eeprom is not configured
    // or if the button is held and we've just powered on
    // If it's not configured the EEPROM contains 0xFF in each location
    if ((DRY_POINT == 0xFFFF && WET_POINT == 0xFFFF) || (digitalRead(BUTTON_PIN) == LOW && isPowerOnReset)) {
      calibrate();
    }
  }

  void writeConfig() {
    EEPROM.put(DRY_POINT_ADDR, DRY_POINT);
    EEPROM.put(WET_POINT_ADDR, WET_POINT);
    EEPROM.put(NORMALISED_WATER_POINT_ADDR, NORMALISED_WATER_POINT);
  }
}
