// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain
// DHT_dual_test
// Demonstrates multiple sensors
// Modified sketch by DIY-SciB.org

///////////  *************** librerie  *****************

#include "DHT.h"
#define DHT1PIN 2  // inserire il numero del pin a cui collegato sensore => in questo caso sostituisci il 2
#define DHT2PIN 3  //  come sopra ma sostituire il 3

// Uncomment whatever type you're using!
#define DHT1TYPE DHT11 // DHT 11
#define DHT2TYPE DHT22 // DHT 22 (AM2302)  /// toglila te usi il dht11
//#define DHTTYPE DHT21 // DHT 21 (AM2301) /// toglila te usi il dht11


// *******************   suggerimenti su come collegare il sensore  ****************
// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

DHT dht1(DHT1PIN, DHT1TYPE);
DHT dht2(DHT2PIN, DHT2TYPE);

/// **************  Inizio il VOID SETUP

void setup() {
Serial.begin(9600);
Serial.println("DHTxx test!");
dht1.begin();
dht2.begin();
}

/// **************  Inizio il void loop

void loop() {
// Reading temperature or humidity takes about 250 milliseconds!
// Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
float h1 = dht1.readHumidity();
float t1 = dht1.readTemperature();
float h2 = dht2.readHumidity();
float t2 = dht2.readTemperature();
// check if returns are valid, if they are NaN (not a number) then something went wrong!
if (isnan(t1) || isnan(h1)) {
Serial.println("Failed to read from DHT #1");
} else {
Serial.print("Humidity 1: ");
Serial.print(h1);
Serial.print(" %\t");
Serial.print("Temperature 1: ");
Serial.print(t1);
Serial.println(" *C");
}
if (isnan(t2) || isnan(h2)) {
Serial.println("Failed to read from DHT #2");
} else {
Serial.print("Humidity 2: ");
Serial.print(h2);
Serial.print(" %\t");
Serial.print("Temperature 2: ");
Serial.print(t2);
Serial.println(" *C");
}
 /// finito di cambiare le varie cose puoi caricarlo e provarlo.......  :smiley-cool:  
