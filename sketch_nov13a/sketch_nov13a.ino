#include <IRremote.h>
int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;
void setup()
{
 Serial.begin(9600);
 irrecv.enableIRIn(); // Start the receiver
}
void loop() {
 if (irrecv.decode(&results)) {
   if(results.decode_type==NEC) Serial.println("NEC");
 else if(results.decode_type==SONY) Serial.println("SONY");
 else if(results.decode_type==RC5) Serial.println("RC5");
 else if(results.decode_type==RC6) Serial.println("RC6");
else if(results.decode_type==SHARP) Serial.println("SHARP");
else  Serial.println(results.decode_type);
 Serial.println(results.value, HEX);
 irrecv.resume(); // Receive the next value
 }
}
