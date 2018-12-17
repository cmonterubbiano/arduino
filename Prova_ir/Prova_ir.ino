#include <IRremote.h>
int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
IRsend irsend; //usare pin 3
decode_results results;

unsigned int S_pwr[68]={4600,4350,700,1550,650,1550,650,1600,650,450,650,450,650,450,650,450,700,400,700,1550,650,1550,650,1600,650,450,650,450,650,450,700,450,650,450,650,450,650,1550,700,450,650,450,650,450,650,450,650,450,700,400,650,1600,650,450,650,1550,650,1600,650,1550,650,1550,700,1550,650,1550,650};

void setup()
{
 Serial.begin(9600);
 irrecv.enableIRIn();
}
void loop() 
{
 if (irrecv.decode(&results)) 
 {
   Serial.println("ON");
   irsend.sendRaw(S_pwr,68,38);
   delay(50);
   irrecv.enableIRIn();
  irrecv.resume();
 } //if 
} // loop
