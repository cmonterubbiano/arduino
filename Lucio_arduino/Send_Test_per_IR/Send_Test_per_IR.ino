/*
  Example for different sending methods
  
  http://code.google.com/p/rc-switch/
  
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {

  Serial.begin(9600);
  pinMode(13, OUTPUT);
  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(10);
    // Optional set pulse length.
  mySwitch.setPulseLength(350);
  delay(1000);

  
}

void loop()
{
  int  ix;
  
  digitalWrite(13, HIGH);
  for (ix =0; ix <4; ix++)
  {
    mySwitch.send(11010, 24);
    delay(1000);
    if (!ix)
      digitalWrite(13, LOW);
  }
  delay(6000);
  digitalWrite(13, HIGH);
  for (ix =0; ix <4; ix++)
  {
    mySwitch.send(11210, 24);
    delay(1000);
    if (ix ==1)
      digitalWrite(13, LOW);
  }
  delay(6000);
  digitalWrite(13, HIGH);
  for (ix =0; ix <4; ix++)
  {
    mySwitch.send(11410, 24);
    delay(1000);
    if (ix ==2)
      digitalWrite(13, LOW);
  }
  delay(6000);
   digitalWrite(13, HIGH);
  for (ix =0; ix <4; ix++)
  {
    mySwitch.send(11610, 24);
    delay(1000);
    if (ix ==3)
      digitalWrite(13, LOW);
  }
  delay(6000);
}
