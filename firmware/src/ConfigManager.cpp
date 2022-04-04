#include <EEPROM.h>

// Handles storing data in the EEPROM
// MAP
#define DRY_POINT_ADDR 0
#define WET_POINT_ADDR 2
#define NORMALISED_WATER_POINT_ADDR 4

namespace ConfigManager {
  int DRY_POINT;// = 3200;
  int WET_POINT;// = 1500;
  int NORMALISED_WATER_POINT;// = 3;

  void loadConfig() {
    // Reads the configuration from EEPROM
    EEPROM.get(DRY_POINT_ADDR, DRY_POINT);
    EEPROM.get(WET_POINT_ADDR, WET_POINT);

    EEPROM.get(NORMALISED_WATER_POINT_ADDR, NORMALISED_WATER_POINT);
  }

  void writeConfig() {
    EEPROM.put(DRY_POINT_ADDR, 3200);
    EEPROM.put(WET_POINT_ADDR, 1500);
    EEPROM.put(NORMALISED_WATER_POINT_ADDR, 4);
  }
}
