/*
  Simple example for receiving
  
  http://code.google.com/p/rc-switch/
*/

#include <RCSwitch.h>
#include <IRremote.h>

IRsend irsend;

//You could also try 36,40 & 56 frequencies as a test NORMAL = 38
int khz = 38; //NB Change this default value as neccessary to the correct modulation frequency

RCSwitch mySwitch = RCSwitch();

unsigned  long  signal_0 []={0xF20D13EC,0x1604000,0x21000000};
unsigned  long  signal_1 []={0xF20D13EC,0x1604700,0x26000000};

int led = 13;
int ix;


void setup()
{
  Serial.begin(9600);
  irsend.enableIROut(khz);
  delay(1000);
  Serial.println(F("AnalysIR"));
  pinMode(13, OUTPUT);
}

void loop()
{  
  powerOnAndHIgh();
  powerOnOff();
}





void powerOnAndHIgh()
{
    digitalWrite(led, HIGH);
    irsend.sendToshiba(signal_0[0], signal_0[1], signal_0[2]);
    delay(500);
    digitalWrite(led, LOW);
    
    delay(4500);
}

void powerOnOff()
{
    digitalWrite(led, HIGH);
    irsend.sendToshiba(signal_1[0], signal_1[1], signal_1[2]);
    delay(1000);
    digitalWrite(led, LOW);
    
    delay(4000);
}

