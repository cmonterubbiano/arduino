
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
int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;
int  ciclo =0;
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
  
  Serial.print("Valore del codice x test = ");
  Serial.print(results->value, DEC);
  delay(10);
  if (ciclo)
    Serial.println(" -- ");
  else
    Serial.print(" -- ");
  ciclo =!ciclo;    
}

void loop()
{
  if (irrecv.decode(&results))
  {
    dump(&results);
    irrecv.resume(); // Receive the next value
  }
}
