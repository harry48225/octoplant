#ifndef MOISTURE_MANAGER
#define MOISTURE_MANAGER

namespace MoistureManager {
  extern void setup();
  extern int getNormalisedReading(); // Returns moisture between 0-12 inclusive
}

#endif