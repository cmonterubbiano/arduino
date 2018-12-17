/*
  Example for different sending methods
  
  http://code.google.com/p/rc-switch/
  
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
  // mySwitch.setRepeatTransmit(15);
  Serial.print("test ");
  
}

void loop() {
  if (mySwitch.available()) 
  {
    Serial.print("Received ");
    Serial.println( mySwitch.getReceivedValue() );
    
    if (mySwitch.getReceivedValue() ==12670912)
        mySwitch.send(983232, 24);  //  presa 2 ON
    if (mySwitch.getReceivedValue() ==12670732)
        mySwitch.send(983040, 24);  //  presa 2 OFF
  }  
  
  mySwitch.resetAvailable();            
}
