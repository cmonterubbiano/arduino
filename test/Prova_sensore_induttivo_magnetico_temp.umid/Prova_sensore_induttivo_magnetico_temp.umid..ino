/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://www.arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */
#include "DHT.h"

#define DHTPIN0 2     // what pin we're connected to
// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
// the setup function runs once when you press reset or power the board
DHT dht0(DHTPIN0, DHTTYPE);

float t1min =.0;
float t1max =.0;
float u1min =.0;
float u1max =.0;

float t2min =.0;
float t2max =.0;
float u2min =.0;
float u2max =.0;

void setup()
{
  Serial.begin(9600);
  dht0.begin();
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  digitalWrite(9, HIGH);
}

// the loop function runs over and over again forever
void loop() 
{
   // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht0.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht0.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht0.readTemperature(true);

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
    Serial.println("Failed to read from DHT0 sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht0.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht0.computeHeatIndex(t, h, false);

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

  int  pulstat =digitalRead(8);
  digitalWrite(13, LOW);
  delay(500);
  if (!pulstat)
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)h
  Serial.print("Sensore induttivo : ");
  Serial.print(pulstat);
  delay(1000);
  digitalWrite(13, LOW);
  delay(500);
  pulstat =digitalRead(9);
  Serial.print(" - Sensore magnetico : ");
  Serial.println(pulstat);
  if (!pulstat)
  {
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(400);
    digitalWrite(13, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(200);
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(400);
  }
  else  delay(1000);
}
