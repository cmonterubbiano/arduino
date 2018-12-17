/*
  Arduino & OpenEnergyMonitor 
  Test & Calibration sketch
  author Mirco Piccin aka pitusso
*/

// Include Emon Library
#include "EmonLib.h"                   

//Calibrations
const int volt = 220;
// nuova_calibrazione = (lettura_corretta / lettura_arduino) * attuale_calibrazione
//const float ct_calibration = 29;
const float ct_calibration = 48.5;

//Sensor pins
const int currentSensorPin = A2;

float Irms = 0;

// Create an Emon instance
EnergyMonitor emon1;                   

void setup() {
  // start serial port:
  Serial.begin(9600);
  Serial.println("Sensors test starting...");

  //initialize Emon instance
  emon1.current(currentSensorPin, ct_calibration); 
}

void loop() {
  
  //read sensors
  Irms = emon1.calcIrms(1480);

  Serial.print("Power : ");
  Serial.println(Irms*volt);
}
