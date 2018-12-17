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
  mySwitch.enableTransmit(10);

  // Optional set pulse length.
  // Recive demo advanced dice 231 millsecondi
  mySwitch.setPulseLength(230);
  
  // Optional set protocol (default is 1, will work for most outlets)
  // mySwitch.setProtocol(2);
  
  // Optional set number of transmission repetitions.
  // mySwitch.setRepeatTransmit(15);
  
}

void loop()
{
  for(int  ix =0; ix <26; ix++)
  {
    if (ix <10)
          mySwitch.setPulseLength(230);
    else  mySwitch.setPulseLength(320);
    
    Serial.println(ix);
    for (int iy =0;iy <2;iy++)
    {
      switch  (ix)
      {
        case  0:
              mySwitch.send(4128960, 24);  // presa 1 ON
              break;
        case  1:
              mySwitch.send(4128768, 24);  // presa 1 OFF
              break;
        case  2:
              mySwitch.send(983232, 24);  //  presa 2 ON
              break;
        case  3:
              mySwitch.send(983040, 24);  //  presa 2 OFF
              break;
        case  4:
              mySwitch.send(3342528, 24);  //  presa 3 ON
              break;
        case  5:
              mySwitch.send(3342336, 24);  //  presa 3 OFF
              break;
        case  6:
              mySwitch.send(196800, 24);  //  presa 4 ON
              break; 
        case  7:
              mySwitch.send(196608, 24);  //  presa 4 OFF
              break; 
        case  8:
             mySwitch.send(3932352, 24);  //  presa 5 ON
             break;
        case  9:
             mySwitch.send(3932160, 24);  //  presa 5 ON
             break;
        case  10:
             mySwitch.send(11010, 24);  //  presa 5 ON
             break;
        case  11:
             mySwitch.send(11210, 24);  //  presa 5 ON
             break;
        case  12:
             mySwitch.send(11410, 24);  //  presa 5 ON
             break;
        case  13:
             mySwitch.send(11610, 24);  //  presa 5 ON
             break;
        case  14:
             mySwitch.send(21010, 24);  //  presa 5 ON
             break;
        case  15:
             mySwitch.send(21210, 24);  //  presa 5 ON
             break;
        case  16:
             mySwitch.send(21410, 24);  //  presa 5 ON
             break;
        case  17:
             mySwitch.send(21610, 24);  //  presa 5 ON
             break;
        case  18:
             mySwitch.send(31010, 24);  //  presa 5 ON
             break;
        case  19:
             mySwitch.send(31210, 24);  //  presa 5 ON
             break;
        case  20:
             mySwitch.send(31410, 24);  //  presa 5 ON
             break;
        case  21:
             mySwitch.send(31610, 24);  //  presa 5 ON
             break;
        case  22:
             mySwitch.send(41010, 24);  //  presa 5 ON
             break;
        case  23:
             mySwitch.send(41210, 24);  //  presa 5 ON
             break;
        case  24:
             mySwitch.send(41410, 24);  //  presa 5 ON
             break;
        case  25:
             mySwitch.send(41610, 24);  //  presa 5 ON
             break;
       }          
    }
    delay(1000);
  }              
}
