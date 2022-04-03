#include "MoistureManager.h"
#include <Arduino.h>

namespace MoistureManager {
  const int _sensorOutput = PIN_PA1;
  const int _sensorInput = PIN_PA3;

  int _DRY_POINT = 3200;
  int _WET_POINT = 1500;

  void setup() {
    analogReadResolution(12);
    pinMode(_sensorOutput, INPUT);
    pinMode(_sensorInput, OUTPUT);
    TCA0.SPLIT.CTRLB = TCA_SPLIT_HCMP0EN_bm; // PWM on WO3 - PA3
    TCA0.SPLIT.HPER = 0x02; // Count all the way down from 0x02 (2) At 10MHz, this gives ((10MHz / 64)/2) ~80KHz PWM
    TCA0.SPLIT.HCMP0  = 0x01; // 50% duty cycle
  }

  int getNormalisedReading() {
    int moisture = analogRead(_sensorOutput);
    moisture -= _WET_POINT;
    moisture /= ((_DRY_POINT - _WET_POINT)/12);
    moisture = 12 - constrain(moisture, 0, 12);

    return moisture;
  }
}