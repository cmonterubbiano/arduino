
/** Modified by Chris Targett* Now includes more protocols
* Novemeber 2011

 

* IRremote: IRrecvDump – dump details of IR codes with IRrecv

* An IR detector/demodulator must be connected to the input RECV_PIN.

* Version 0.1 July, 2009

* Copyright 2009 Ken Shirriff

* http://arcfn.com

*

* Modified by Chris Targett to speed up the process of collecting

* IR (HEX and DEC) codes from a remote (to put into and .h file)

*

*/

#include <IRremote.h>
IRsend irsend;
int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;

int khz=38; //NB Change this default value as neccessary 
            //to the correct modulation frequency
//unsigned acceso [] ={4350,4250,600,1550,550,1600,550,1550,600,1550,550,500,600,450,600,1550,550,500,600,500,550,500,
//                    550,500,600,450,600,1550,600,1550,550,500,550,1550,600,500,550,500,600,450,600,1550,600,450,600,
//                    500,550,1550,600,1550,550,1550,600,1550,600,1550,550,500,550,1600,550,1550,600,450,600,500,550,
//                    500,600,450,600,500,550,500,550,500,600,500,550,500,550,1550,600,500,550,1550,600,1550,550,1600,
//                    550,500,550,500,600,450,600,500,550};
unsigned tasto_b[]  ={4500,4350,600,1600,550,1600,600,1600,600,550,550,550,550,500,600,500,600, 
                    550,600,1550,650,1550,600,1550,650,550,550,500,600,500,550,550,550,600,600,450,650,
                    1550,600,1550,600,600,550,1600,600,500,550,550,550,550,650,1550,600,450,650,450,600,
                    1650,600,500,600,1550,600,1600,600,1550,600}; 


void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}
// Dumps out the decode_results structure.
// Call this after IRrecv::decode()
// void * to work around compiler issue
//void dump(void *v) {
//  decode_results *results = (decode_results *)v
void dump(decode_results *results)
{
  int count = results->rawlen;
  if (results->decode_type == UNKNOWN)
  {
    Serial.print("Unknown encoding: ");
  }
  else if (results->decode_type == NEC)
  {
    Serial.print("Decoded NEC: ");
  }
  else if (results->decode_type == SONY)
  {
    Serial.print("Decoded SONY: ");
  }
  else if (results->decode_type == RC5)
  {
    Serial.print("Decoded RC5: ");
  }
  else if (results->decode_type == RC6)
  {
    Serial.print("Decoded RC6: ");
  }
  else if (results->decode_type == SAMSUNG)
  {
    Serial.print("Decoded SAMSUNG: ");
  }
  else if (results->decode_type == JVC)
  {
    Serial.print("Decoded JVC: ");
  }
  else if (results->decode_type == PANASONIC)
  {
    Serial.print("Decoded Panasonic: ");
  }
  Serial.print(results->value, HEX);
  Serial.print("(");
  Serial.print(results->bits, DEC);
  Serial.println(" bits)");
  Serial.print("#define Something_DEC ");
  Serial.println(results->value, DEC);
  Serial.print("#define Something_HEX ");
  Serial.println(results->value, HEX);
  Serial.print("Raw (");
  Serial.print(count, DEC);
  Serial.print("): ");
  for (int i = 0; i < count; i++)
  {
    if ((i % 2) == 1)
    {
      Serial.print(results->rawbuf[i]*USECPERTICK, DEC);
    }
    else
    {
      Serial.print(-(int)results->rawbuf[i]*USECPERTICK, DEC);
    }
    Serial.print(" ");
  }
  Serial.println("");
  Serial.print("Traduzione [] ={");
  for (int i = 1; i < count; i++)
  {
    if (i >1)
      Serial.print(",");
    Serial.print(results->rawbuf[i]*USECPERTICK, DEC);
  }
  Serial.println("};");
}

void loop()
{
  if (Serial.available())
  {
    //leggi il carattere in arrivo
    byte data = Serial.read();
 
    if(data == 't') //trasmetti acceso
    {
      Serial.println("Accendo .....");
      irsend.sendRaw(tasto_b, sizeof(tasto_b)/sizeof(int), khz);
      delay(50)
      irrecv.enableIRIn(); // Start the receiver
    }
  }
  
  if (irrecv.decode(&results))
  {
    dump(&results);
    irrecv.resume(); // Receive the next value
  }
}
