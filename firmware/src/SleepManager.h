#ifndef SLEEP_MANAGER
#define SLEEP_MANAGER

namespace SleepManager {
  extern void sleep(); // Puts the device to sleep
  extern void setup();
  extern volatile unsigned long wakeTime;
}

#endif