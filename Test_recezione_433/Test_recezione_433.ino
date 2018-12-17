/*
  Simple example for receiving
  
  http://code.google.com/p/rc-switch/
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();
long  ix =0L;
long  ultimo =0L;
int  persi =0;

void setup() {
  Serial.begin(9600);
  mySwitch.enableReceive(0);  // Receiver on inerrupt 0 => that is pin #2
}

void loop() 
{
  if (mySwitch.available()) 
  {  
    int value = mySwitch.getReceivedValue();
    ix++;
    if (mySwitch.getReceivedValue() ==4128960)
    {
      int  diff =0;
      
      if (ultimo)
      {
        int  appo;
        
        for (appo =ultimo; appo <ix; appo +=26);
        diff =appo -ix;;
      }
      persi +=diff;
      Serial.print("---------------------------------------------Segnali persi ");
      Serial.print(diff);
      Serial.print(" Totale ");
      Serial.println(persi);
      ultimo =ix;
    }
    Serial.print(ix);
    Serial.print(" ");

    Serial.print("Received ");
    Serial.print( mySwitch.getReceivedValue() );
    Serial.print(" / ");
    Serial.print( mySwitch.getReceivedBitlength() );
    Serial.print("bit ");
    Serial.print("Protocol: ");
    Serial.println( mySwitch.getReceivedProtocol() );
    delay(800);

    mySwitch.resetAvailable();
  }
}
