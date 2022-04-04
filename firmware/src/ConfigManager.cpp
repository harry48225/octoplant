#include "ConfigManager.h"

#include <EEPROM.h>
#include <LedManager.h>
#include <Arduino.h>
#include "MoistureManager.h"

// Handles storing data in the EEPROM
// MAP
#define DRY_POINT_ADDR 0
#define WET_POINT_ADDR 2
#define NORMALISED_WATER_POINT_ADDR 4

namespace ConfigManager {
  int DRY_POINT;// = 3200;
  int WET_POINT;// = 1500;
  int NORMALISED_WATER_POINT;// = 3;

  void calibrate() {
    // Enable button
    pinMode(PIN_PA2, INPUT_PULLUP);
    // Set the wet point
    for (int i = 0; i < 12; i++) {
      LedManager::flashLed(i, LedManager::FlashRate::FAST);
    }

    while (digitalRead(PIN_PA2) == LOW) {
      delay(10);
    }

    while (digitalRead(PIN_PA2) == HIGH) {
      delay(10);
    }

    WET_POINT = MoistureManager::getRawReading() + 100;

    while (digitalRead(PIN_PA2) == LOW) {
      delay(10);
    }

    LedManager::resetLeds();

    LedManager::flashLed(11, LedManager::FlashRate::FAST);

    delay(100);

    while(digitalRead(PIN_PA2) == HIGH) {
      delay(10);
    }

    DRY_POINT = MoistureManager::getRawReading() - 100;

    while (digitalRead(PIN_PA2) == LOW) {
      delay(10);
    }

    LedManager::resetLeds();

    for (int i = 0; i <= 6; i++) {
      LedManager::turnOnLed(6-i);
      LedManager::turnOnLed(5+i);
      delay(20);
    }

    LedManager::resetLeds();

    writeConfig();
  }

  void loadConfig() {
    // Reads the configuration from EEPROM
    EEPROM.get(DRY_POINT_ADDR, DRY_POINT);
    EEPROM.get(WET_POINT_ADDR, WET_POINT);

    EEPROM.get(NORMALISED_WATER_POINT_ADDR, NORMALISED_WATER_POINT);

    // Enable button
    pinMode(PIN_PA2, INPUT_PULLUP);

    // Calibrate if the eeprom is not configured
    // or if the button is held
    // If it's not configured the EEPROM contains 0xFF in each location
    if ((DRY_POINT == 0xFFFF && WET_POINT == 0xFFFF) || digitalRead(PIN_PA2) == LOW) {
      calibrate();
    }
  }

  void writeConfig() {
    EEPROM.put(DRY_POINT_ADDR, DRY_POINT);
    EEPROM.put(WET_POINT_ADDR, WET_POINT);
    EEPROM.put(NORMALISED_WATER_POINT_ADDR, 4);
  }
}
