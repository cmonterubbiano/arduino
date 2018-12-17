
/*
Author: AnalysIR 2004 - http://www.AnalysIR.com/
This sketch is Free to use - without restriction
*/

#include <IRremote.h>
#include <RCSwitch.h>

IRsend irsend;
RCSwitch mySwitch = RCSwitch();

//You could also try 36,40 & 56 frequencies as a test NORMAL = 38
int khz = 38; //NB Change this default value as neccessary to the correct modulation frequency

//toshiba accensione
unsigned int Signal_0_0[] = {4416, 4312, 544, 1604, 552, 1604, 568, 1584, 544, 1604, 544, 532, 548, 528, 572, 1580, 548, 528, 544, 532, 544, 532, 544, 532, 540, 560, 544, 1580, 572, 1584, 540, 532, 544, 1608, 572, 504, 544, 532, 544, 532, 544, 1608, 568, 508, 544, 532, 540, 1608, 544, 1608, 548, 1604, 568, 1584, 544, 1604, 544, 532, 572, 1580, 548, 1604, 568, 508, 568, 508, 544, 528, 548, 532, 540, 532, 548, 528, 548, 532, 568, 504, 544, 536, 564, 1584, 544, 532, 540, 1612, 544, 1608, 568, 508, 540, 536, 544, 528, 572, 504, 548, 528, 548, 528, 548, 1604, 568, 508, 568, 508, 544, 532, 572, 504, 540, 1608, 572, 1584, 540, 532, 544, 532, 544, 532, 544, 532, 540, 536, 540, 532, 548, 528, 548, 532, 568, 508, 568, 508, 540, 1608, 572, 504, 544, 532, 544, 532, 544, 1608, 568, 508, 544, 7428, 4360, 4340, 544, 1608, 568, 1584, 568, 1584, 540, 1608, 544, 532, 548, 528, 544, 1608, 544, 532, 544, 532, 544, 532, 540, 536, 540, 532, 548, 1608, 544, 1608, 564, 508, 572, 1580, 540, 536, 544, 528, 572, 508, 544, 1608, 540, 536, 540, 532, 572, 1580, 548, 1628, 544, 1584, 568, 1584, 540, 1612, 568, 504, 572, 1584, 564, 1608, 548, 504, 572, 504, 572, 504, 540, 536, 568, 504, 572, 508, 568, 508, 544, 532, 540, 536, 540, 1608, 544, 532, 544, 1608, 544, 1608, 544, 556, 520, 556, 520, 556, 544, 508, 540, 532, 544, 532, 544, 1636, 544, 532, 516, 532, 568, 532, 520, 532, 572, 1580, 540, 1612, 568, 504, 572, 504, 572, 508, 568, 508, 540, 536, 540, 532, 544, 532, 540, 564, 512, 536, 544, 528, 572, 1584, 544, 532, 564, 536, 544, 508, 540, 1608, 544, 532, 544};
//toshiba spegnimento
unsigned int Signal_0_1[] = {4388, 4340, 544, 1604, 572, 1580, 572, 1580, 544, 1608, 568, 508, 544, 532, 544, 1604, 572, 508, 544, 532, 540, 532, 568, 508, 544, 532, 572, 1580, 548, 1604, 544, 532, 544, 1604, 572, 504, 544, 532, 572, 504, 572, 1580, 568, 508, 568, 508, 544, 1604, 576, 1576, 572, 1584, 544, 1604, 544, 1608, 544, 532, 572, 1576, 548, 1608, 544, 532, 564, 508, 572, 504, 544, 532, 572, 504, 548, 524, 548, 532, 544, 532, 544, 532, 568, 1580, 544, 532, 572, 1584, 540, 1608, 544, 532, 572, 504, 572, 504, 568, 504, 572, 508, 568, 508, 568, 1580, 572, 504, 572, 504, 572, 504, 572, 1604, 544, 1584, 572, 1608, 512, 532, 548, 532, 544, 532, 544, 532, 540, 532, 544, 532, 572, 504, 544, 532, 548, 528, 544, 532, 544, 1608, 568, 508, 540, 536, 568, 1580, 572, 1580, 548, 532, 540, 7428, 4364, 4340, 540, 1636, 544, 1580, 548, 1600, 572, 1584, 544, 532, 540, 536, 540, 1608, 572, 504, 572, 532, 516, 536, 564, 508, 568, 508, 572, 1580, 544, 1604, 572, 504, 548, 1608, 540, 536, 540, 556, 544, 508, 544, 1608, 540, 532, 548, 528, 548, 1632, 520, 1608, 568, 1580, 544, 1608, 544, 1608, 568, 508, 544, 1608, 568, 1580, 548, 528, 548, 532, 564, 512, 540, 556, 520, 532, 544, 532, 544, 528, 572, 504, 572, 508, 544, 1608, 540, 532, 544, 1608, 544, 1608, 572, 508, 540, 532, 568, 508, 544, 532, 596, 480, 572, 504, 540, 1608, 572, 504, 572, 508, 544, 532, 564, 1584, 544, 1608, 540, 1612, 544, 532, 544, 532, 544, 532, 544, 556, 540, 512, 540, 532, 572, 504, 544, 532, 544, 536, 564, 508, 568, 1584, 544, 532, 544, 528, 572, 1580, 548, 1604, 544, 532, 572};
unsigned int  P[295];
void setup()
{
  Serial.begin(9600);
   mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
                               // Receiver on interrupt 1 => that is pin #3
  irsend.enableIROut(khz);
  delay(1000);
  Serial.println(F("AnalysIR"));
  pinMode(13, OUTPUT);
}

void loop()
{
  
  if (mySwitch.available())
  {
    int value = mySwitch.getReceivedValue();
    
    if (value == 0)
      Serial.print("Unknown encoding");
    else
    {
      long  codice =mySwitch.getReceivedValue();
      int  ix;
      
      digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
      Serial.print("Received ");
      Serial.print(codice);
      Serial.print(" / ");
      Serial.print( mySwitch.getReceivedBitlength() );
      Serial.print("bit ");
      Serial.print("Protocol: ");
      Serial.println( mySwitch.getReceivedProtocol() );
      if (codice ==12844224L)
      {
        Serial.println("Tasto Accendi");
        for (ix =0; ix <sizeof(Signal_0_0); ix++)
          P[ix] =Signal_0_0[ix];
          
        irsend.sendRaw(P, sizeof(P)/sizeof(int), khz); //AnalysIR Batch Export (IRremote) - RAW
      } 
      if (codice ==12844044L)
      {
        Serial.println("Tasto spegni");
        //irsend.sendRaw(Signal_0_1, sizeof(Signal_0_0)/sizeof(int), khz); //AnalysIR Batch Export (IRremote) - RAW
      } 
      if (codice ==12844035L)
      {
        Serial.println("Tasto C");
      } 
      if (codice ==13894668L)
      {
        Serial.println("Tasto D");
      }         
 // irsend.sendRaw(Signal_0_0, sizeof(Signal_0_0)/sizeof(int), khz); //AnalysIR Batch Export (IRremote) - RAW
   // AnalysIR IR Protocol: MITSUBISHI288AC, Key:  
 
  //delay(500);              // wait for a 1/2 second
  digitalWrite(13, LOW);  // turn the LED off by making the voltage LOW     

//  delay(10000); //send signal every 10 seconds for testing
//  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
//  irsend.sendRaw(Signal_0_1, sizeof(Signal_0_0)/sizeof(int), khz); //AnalysIR Batch Export (IRremote) - RAW
//  delay(500);              // wait for a 1/2 second
//  digitalWrite(13, LOW);  // turn the LED off by making the voltage LOW     
//
//  delay(10000); //send signal every 10 seconds for testing
    }
    mySwitch.resetAvailable();
  }
}
