#include <OneWire.h>

// OneWire DS18S20, DS18B20, DS1822 Temperature Example
//
// http://www.pjrc.com/teensy/td_libs_OneWire.html
//
// The DallasTemperature library can do all this work for you!
// http://milesburton.com/Dallas_Temperature_Control_Library
// confermo pin 10 resistenza da 4,7 Khom 
OneWire  ds(10);  // on pin 10 (a 4.7K resistor is necessary)

byte addr[8] = {0x28,0x77,0xDB,0x8B,0x03,0x00,0x00,0x84};
float  minimo =100;
float  massimo = -100;
float  ultima;
int  volte =0;
float  lettmin =100;
float  lettmax = -100;
float  sommale =0;

void setup(void) 
{
  Serial.begin(9600);
}

void loop(void) 
{
  byte i;
  byte data[12];
  float celsius, fahrenheit;
  int  nosomma =0;
  
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
  
  if (celsius <lettmin)
  {
    if (lettmin !=100)
      sommale +=lettmin;
    lettmin =celsius;
    nosomma =1;
  }
  else  if (celsius >lettmax)
  {
    if (lettmax != -100)
      sommale +=lettmax;
    lettmax =celsius;
    nosomma =1;
  }
  if (!nosomma)
    sommale +=celsius;
  volte++;
//  {
//      Serial.print("  Temperatura = ");
//      Serial.print(celsius);
//      Serial.print(" Celsius, ");
//      Serial.print(lettmin);
//      Serial.print(" Minima, ");
//      Serial.print(lettmax);
//      Serial.print(" Massima, ");
//      Serial.print(sommale);
//      Serial.print(" Somma, ");
//      Serial.print(volte);
//      Serial.println(" Numero");
//  }
  if (volte ==10)
  {
    celsius =(sommale /8);

    if (celsius <minimo)
      minimo =celsius;
    if (celsius >massimo)
      massimo =celsius;
    fahrenheit = celsius * 1.8 + 32.0;
//    if (ultima !=celsius)
    {
      Serial.print("  Temperatura = ");
      Serial.print(celsius);
      Serial.print(" Celsius, ");
//    Serial.print(fahrenheit);
//    Serial.print(" Fahrenheit, ");
      Serial.print(minimo);
      Serial.print(" Minima, ");
      Serial.print(massimo);
      Serial.println(" Massima");
      ultima =celsius;
      volte=0;
      lettmin =100;
      lettmax = -100;
      sommale =0;
    }
  }
}
