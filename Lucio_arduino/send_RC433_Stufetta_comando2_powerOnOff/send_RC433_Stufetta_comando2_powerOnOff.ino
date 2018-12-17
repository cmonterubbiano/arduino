/*
  Example for different sending methods
  
  http://code.google.com/p/rc-switch/
  
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

unsigned int signal1 = 10;
unsigned int signal2 = 210;
unsigned int signal3 = 410;
unsigned int signal4 = 610;
unsigned int incremento = 0;

void setup() {

// if (!strcmp(nomeRicettore, "Condizionatore Sala"))
//  incremento =11000;
//  if (!strcmp(nomeRicettore, "Condizioantore Camera Sud"))
//    incremento =21000;
//  if (!strcmp(nomeRicettore, "Condizioantore Camera Sud"))
//    incremento =31000;
//  if (!strcmp(nomeRicettore, "Stufa Bagno"))
    incremento =41000; 
  
  
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(10);
    // Optional set pulse length.
  mySwitch.setPulseLength(350);
  delay(1000);


  sendSignal(signal3);
  
}

void loop()
{

}

void sendSignal(unsigned int signal)
{
  int  ix;
  Serial.print(F("sending 433 "));
  
  digitalWrite(13, HIGH);
  for (ix =0; ix <4; ix++)
  {
    mySwitch.send((signal + incremento), 24);
    delay(1000);
    if (!ix)
      digitalWrite(13, LOW);
  }
}
