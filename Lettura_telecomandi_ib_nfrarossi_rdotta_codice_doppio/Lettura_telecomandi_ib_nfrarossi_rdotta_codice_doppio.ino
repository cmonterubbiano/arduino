#include <IRremote.h>
int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;
decode_results result1;
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
  Serial.println(results->value, DEC);

  Serial.print("Copiare--> [] ={");
  for (int i = 1; i < count; i++)
  {
    if (i >1)
      Serial.print(",");
    Serial.print(results->rawbuf[i]*USECPERTICK, DEC);
  }
  Serial.println("};");
}
void dump1(decode_results *result1)
{
  int count = result1->rawlen;
  
  Serial.print("Valore del codice x test2= ");
  Serial.println(result1->value, DEC);

  Serial.print("Copiare-2> [] ={");
  for (int i = 1; i < count; i++)
  {
    if (i >1)
      Serial.print(",");
    Serial.print(result1->rawbuf[i]*USECPERTICK, DEC);
  }
  Serial.println("};");
}

void loop()
{
  if (!ciclo && irrecv.decode(&results))
  {
    irrecv.resume(); // Receive the next value
    ciclo++;
  }
  if (ciclo && irrecv.decode(&result1))
  {
    dump(&results);
    dump1(&result1);
    irrecv.resume(); // Receive the next value
    ciclo =0;
  }
}
