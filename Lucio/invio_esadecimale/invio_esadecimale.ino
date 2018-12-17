

#include <IRremote.h>

IRsend irsend;

void setup()
{
 Serial.begin(9600);
}

void loop() {
 if (Serial.read() != -1) {
for(int i=0;i<2;i++)
{
   irsend.sendMitsubishi(0x23CB2601,0x00200807,0x30405600,0x00000000,0x000A0000);
}
   delay(2000);
 }
}
