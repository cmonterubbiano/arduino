/*
  Example for different sending methods
  
  http://code.google.com/p/rc-switch/

  questo sketch legge i primi due comandi del telecomando di Valter antifurto
  ed accende con quello di sinitra grande e spegne con quello di destra piccolo
  una presa comandata in 433 impostata al vaolre 2 "983232" e "983040"
  
*/

#include <RCSwitch.h>
int  val =0;

RCSwitch mySwitch = RCSwitch();

void setup() {

  Serial.begin(9600);
  mySwitch.enableReceive(0);  // Receiver on inerrupt 0 => that is pin #2 
  // Transmitter is connected to Arduino Pin #14  
  mySwitch.enableTransmit(14);

  // Optional set pulse length.0
  // Recive demo advanced dice 231 millsecondi
  mySwitch.setPulseLength(230);
  
  // Optional set protocol (default is 1, will work for most outlets)
  // mySwitch.setProtocol(2);
  
  // Optional set number of transmission repetitions.
  mySwitch.setRepeatTransmit(15);
  Serial.println("Test di ricezione e emissione segnale 433 ");
  
}

void loop() {
  if (mySwitch.available()) 
  {
    Serial.print("Ricevuto ");
    Serial.println( mySwitch.getReceivedValue() );
    
    if (mySwitch.getReceivedValue() ==12670912)
    {
        delay(200);
        mySwitch.send(983232, 24);  //  presa 2 ON
        Serial.println("Presa 2 -> ON ");
    }
    if (mySwitch.getReceivedValue() ==12670732)
    {
        delay(200);
        mySwitch.send(983040, 24);  //  presa 2 OFF
        Serial.println("Presa 2 -> OFF");
    }
  }  
  
  mySwitch.resetAvailable();            
}
