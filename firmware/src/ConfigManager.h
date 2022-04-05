#ifndef CONFIG_MANAGER
#define CONFIG_MANAGER

namespace ConfigManager {
  extern int DRY_POINT;
  extern int WET_POINT;
  extern int NORMALISED_WATER_POINT;

  extern void loadConfig();
  extern void writeConfig();
  extern void setWaterPoint();
}

#endif