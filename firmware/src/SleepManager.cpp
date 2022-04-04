#include <Arduino.h>
#include <avr/sleep.h>
#include "SleepManager.h"
#include "LedManager.h"

namespace SleepManager {
  void sleep() {
    for (int i = 0; i < 12; i++) {
      LedManager::turnOffLed(i);
      delay(25);
    }
    delay(100); // Give the led loop time to turn off the leds
    TCA0.SPLIT.CTRLA = ~TCA_SPLIT_ENABLE_bm; // Disable timer A
    delay(100);

    // Eliminate unused floating pins
    pinMode(PIN_PB0, OUTPUT);
    pinMode(PIN_PB1, OUTPUT);
    pinMode(PIN_PB2, OUTPUT);
    pinMode(PIN_PB3, OUTPUT);
    
    // Set sensor input as output so it's not floating
    pinMode(PIN_PA1, OUTPUT);
    digitalWrite(PIN_PA3, LOW);
    
    // Drive all LED pins LOW
    pinMode(PIN_PA4, OUTPUT);
    pinMode(PIN_PA5, OUTPUT);
    pinMode(PIN_PA6, OUTPUT);
    pinMode(PIN_PA7, OUTPUT);
    digitalWrite(PIN_PA4, LOW);
    digitalWrite(PIN_PA5, LOW);
    digitalWrite(PIN_PA6, LOW);
    digitalWrite(PIN_PA7, LOW);

    ADC0.CTRLA = ~ADC_ENABLE_bm; // Disable ADC
    delay(10);

    // Enable pin falling interrupt and internal pullup on PIN_PA2
    // This is the button to wake the device
    PORTA.PIN2CTRL |= (PORT_PULLUPEN_bm | PORT_ISC_RISING_gc);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_cpu();
  }

  // ISR handler, gets called when button is pressed
  ISR(PORTA_PORT_vect) {
    PORTA.INTFLAGS = 0xFF; // Clear the int flags
    // Reset the chip
    _PROTECTED_WRITE(RSTCTRL.SWRR,1);
  }
}