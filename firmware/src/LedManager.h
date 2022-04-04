#ifndef LED_MANAGER
#define LED_MANAGER

namespace LedManager {
  extern int _ledState[12];
  extern void setup();
  extern void turnOnLed(int ledNumber);
  extern void turnOffLed(int ledNumber);

  enum FlashRate {NONE, SLOW, MEDIUM, FAST};
  extern void flashLed(int ledNumber, FlashRate rate);

  extern void resetLeds();
}
#endif