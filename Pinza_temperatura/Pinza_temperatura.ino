/*
  Arduino & OpenEnergyMonitor 
  Test & Calibration sketch
  author Mirco Piccin aka pitusso
*/

// Include Emon Library
#include "EmonLib.h"                   
#include <OneWire.h>
//Calibrations
const int volt = 220;
// nuova_calibrazione = (lettura_corretta / lettura_arduino) * attuale_calibrazione
//const float ct_calibration = 29;
const float ct_calibration = 48.5;
int  zero =29;
OneWire  ds(9);  // on pin 9 (a 4.7K resistor is necessary)
byte addr[8] = {0x28,0xF5,0xA8,0xC0,0x03,0x00,0x00,0x57};

//Sensor pins
const int currentSensorPin = A2;

float Irms = 0;

// Create an Emon instance
EnergyMonitor emon1;                   

void setup() {
  // start serial port:
  Serial.begin(9600);
  Serial.println("Sensors test starting...");
  pinMode(currentSensorPin, INPUT);

  //initialize Emon instance
  emon1.current(currentSensorPin, ct_calibration); 
}

void loop() 
{
  byte i;
  byte data[12];
  float celsius, fahrenheit;
  
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 9; i++) 
    data[i] = ds.read();

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];

  byte cfg = (data[4] & 0x60);
  // at lower res, the low bits are undefined, so let's zero them
  if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
  else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
  else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
  //// default is 12 bit resolution, 750 ms conversion time

  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0; 
  //read sensors
  Irms = emon1.calcIrms(1480);

  Serial.print("Power : ");
  Serial.print(Irms*volt -zero);
  Serial.print("  Temperatura = ");
  Serial.print(celsius);
  Serial.print(" Celsius, ");
  Serial.print(fahrenheit);
  Serial.println(" Fahrenheit");
}
