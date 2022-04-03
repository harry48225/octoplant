#include <Arduino.h>

int ledPins[] = {PIN_PA7, PIN_PA6, PIN_PA5, PIN_PA4};
int sensorOutput = PIN_PA1;

void turnOffLeds() {
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], INPUT);  
  }
}

void turnOnLedPins(int high, int low) {
  pinMode(ledPins[high], OUTPUT);
  pinMode(ledPins[low], OUTPUT);
  digitalWrite(ledPins[high], HIGH);
  digitalWrite(ledPins[low], LOW);
}

void turnOnLed(int ledNumber) {
  // Turn off the other LEDS
  turnOffLeds();
  switch (ledNumber) {
    case 0:
      turnOnLedPins(0,1);
      break; 
    case 6:
      turnOnLedPins(0,2);
      break;
    case 10:
      turnOnLedPins(0,3);
      break; 
    case 1:
      turnOnLedPins(1,0);
      break;
    case 2:
      turnOnLedPins(1,2);
      break; 
    case 8:
      turnOnLedPins(1,3);
      break;
    case 7:
      turnOnLedPins(2,0);
      break; 
    case 3:
      turnOnLedPins(2,1);
      break;
    case 4:
      turnOnLedPins(2,3);
      break; 
    case 11:
      turnOnLedPins(3,0);
      break;
    case 9:
      turnOnLedPins(3,1);
      break; 
    case 5:
      turnOnLedPins(3,2);
      break;    
  }
}

void setup() {
  analogReadResolution(12);
  pinMode(sensorOutput, INPUT);
  pinMode(PIN_PA3, OUTPUT);
  TCA0.SPLIT.CTRLB = TCA_SPLIT_HCMP0EN_bm; // PWM on WO3 - PA3
  TCA0.SPLIT.HPER= 0xFF; // Count all the way down from 0x0F (3) At 10MHz, this gives ~2.5Mhz PWM
  TCA0.SPLIT.HCMP0  = 0x0F; // 50% duty cycle
  TCA0.SPLIT.CTRLA = TCA_SPLIT_ENABLE_bm ; //enable the timer with no prescaler
}

int ledState[] = {0,0,0,0,0,0,0,0,0,0,0,0};
int DRYPOINT = 2750;//800;
int WETPOINT = 400;//400;

void loop() {
  int moisture = analogRead(sensorOutput);
  Serial.println(moisture);
   moisture -= WETPOINT;
   moisture /= ((DRYPOINT - WETPOINT)/12);
   moisture = 12 - constrain(moisture, 0, 12);
//   if (moisture > 700) {
//    moisture = 12;
//   } else {
//    moisture = 0;}
  //Serial.println(moisture);
  for (int i = 0; i < 12; i++) {
    if ((12-i) <= moisture) {
      ledState[i] = 1;
    }
    else {
      ledState[i] = 0;
    }
  }
  turnOffLeds();
  for (int x = 0; x < 10; x++) {
    for (int i = 0; i < 12; i++) {
      if (ledState[i] == 1) {
        turnOnLed(i);
      }
      delayMicroseconds(100);
    }
  }
}