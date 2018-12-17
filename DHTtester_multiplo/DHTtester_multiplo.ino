// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"

#define DHTPIN0 2     // what pin we're connected to
#define DHTPIN1 3     // what pin we're connected to
#define DHTPIN2 4     // what pin we're connected to
#define DHTPIN3 5     // what pin we're connected to
#define DHTPIN4 6     // what pin we're connected to
#define DHTPIN5 7     // what pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT camera(DHTPIN0, DHTTYPE);
DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
DHT dht3(DHTPIN3, DHTTYPE);
DHT dht4(DHTPIN4, DHTTYPE);
DHT dht5(DHTPIN5, DHTTYPE);

float t1min =.0;
float t1max =.0;
float u1min =.0;
float u1max =.0;

float t2min =.0;
float t2max =.0;
float u2min =.0;
float u2max =.0;

void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");

  camera.begin();
  dht1.begin();
  dht2.begin();
  dht3.begin();
  dht4.begin();
  dht5.begin();
}

void loop() 
{
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = camera.readHumidity();
  // Read temperature as Celsius (the default)
  float t = camera.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = camera.readTemperature(true);

  if (u1min ==.0 || u1min > h)
    u1min =h;
  if (u1max < h)
    u1max =h;
  if (t1min ==.0 || t1min > t)
    t1min =t;
  if (t1max < t)
    t1max =t;

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from camera sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = camera.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = camera.computeHeatIndex(t, h, false);

  Serial.print("Umidita'-0-: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.print(" *C\t");
  Serial.print("Percepita: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print("\t");
  Serial.print("Umidita' min: ");
  Serial.print(u1min);
  Serial.print("\t");
  Serial.print(" max: ");
  Serial.print(u1max);
  Serial.print("\t");
  Serial.print("Temperatura min: ");
  Serial.print(t1min);
  Serial.print("\t");
  Serial.print(" max: ");
  Serial.println(t1max);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  h = dht1.readHumidity();
  // Read temperature as Celsius (the default)
  t = dht1.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  f = dht1.readTemperature(true);

  if (u2min ==.0 || u2min > h)
    u2min =h;
  if (u2max < h)
    u2max =h;
  if (t2min ==.0 || t2min > t)
    t2min =t;
  if (t2max < t)
    t2max =t;
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT1 sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  hif = dht1.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  hic = dht1.computeHeatIndex(t, h, false);

 
  Serial.print("Umidita'-1-: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print("\t");
  Serial.print("Percepita: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print("\t");
  Serial.print("Umidita' min: ");
  Serial.print(u2min);
  Serial.print("\t");
  Serial.print(" max: ");
  Serial.print(u2max);
  Serial.print("\t");
  Serial.print("Temperatura min: ");
  Serial.print(t2min);
  Serial.print(" \t");
  Serial.print(" max: ");
  Serial.println(t2max);
}
