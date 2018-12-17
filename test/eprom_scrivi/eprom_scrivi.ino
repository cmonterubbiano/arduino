#include <EEPROM.h>
 
int a = -1;
int value =256;
 
void setup()
{
  
  Serial.begin(9600);
  for (int i = 0; i < 1024; i++)
  {
    if (value <0 || value >256)
      a *= -1;
   value =(value +a); 
    EEPROM.write(i, value);
  }
  Serial.print("Finito");
}
 
void loop()
{
}
