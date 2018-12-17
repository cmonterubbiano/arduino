/*
  Example for different sending methods
  
  http://code.google.com/p/rc-switch/
  
*/

#include <RCSwitch.h>
int  val =0;

RCSwitch mySwitch = RCSwitch();

void setup() {

  Serial.begin(9600);
  
  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(14);

  // Optional set pulse length.0
  // Recive demo advanced dice 231 millsecondi
  mySwitch.setPulseLength(230);
  
  // Optional set protocol (default is 1, will work for most outlets)
  // mySwitch.setProtocol(2);
  
  // Optional set number of transmission repetitions.
  // mySwitch.setRepeatTransmit(15);
  
}

void loop() {
  if (Serial.available())
  {
    val = Serial.read();
    
    switch  (val)
    {
      case  '0':
             mySwitch.send(4128960, 24);  // presa 1 ON
             break;
      case  '1':
             mySwitch.send(4128768, 24);  // presa 1 OFF
             break;
      case  '2':
             mySwitch.send(983232, 24);  //  presa 2 ON
             break;
      case  '3':
             mySwitch.send(983040, 24);  //  presa 2 OFF
             break;
      case  '4':
             mySwitch.send(3342528, 24);  //  presa 3 ON
             break;
      case  '5':
             mySwitch.send(3342336, 24);  //  presa 3 OFF
             break;
      case  '6':
             mySwitch.send(196800, 24);  //  presa 4 ON
             break; 
      case  '7':
             mySwitch.send(196608, 24);  //  presa 4 OFF
             break; 
      case  '8':
             mySwitch.send(3932352, 24);  //  presa 5 ON
             break;
      case  '9':
             mySwitch.send(3932160, 24);  //  presa 5 OFF
             break; 
    }
  }              
}
