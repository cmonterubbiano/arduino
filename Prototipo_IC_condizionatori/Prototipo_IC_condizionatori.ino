// Wire Master Writer
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Writes data to an I2C/TWI slave device
// Refer to the "Wire Slave Receiver" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>
int  val =0;
int led =13;

void setup()
{
  pinMode ( led , OUTPUT );  
  Serial.begin(9600);
  Wire.begin(); // join i2c bus (address optional for master)
}

void loop()
{
  if (Serial.available())
  {
    val = Serial.read();
    
    switch  (val)
    {
      case  '0':
             digitalWrite ( led , HIGH );
             Serial.println("ON");
             break; 
      case  '1':
             digitalWrite ( led , LOW );
             Serial.println("OFF");
             break; 
      case  '2':
              Serial.println("ON - 1");
              Wire.beginTransmission(1);
              Wire.write("a");  
              Wire.endTransmission();
              break;
      case  '3':
              Serial.println("OF - 1");
              Wire.beginTransmission(1);
              Wire.write("s");  
              Wire.endTransmission();
              break;
      case  '4':
              Serial.println("ON - 2");
              Wire.beginTransmission(2);
              Wire.write("a");  
              Wire.endTransmission();
              break;
      case  '5':
              Serial.println("OF - 2");
              Wire.beginTransmission(2);
              Wire.write("s");  
              Wire.endTransmission();
              break;
      default:
              Serial.println("Default");
              break;
    }
    }
}
