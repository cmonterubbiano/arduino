#include <IRremote.h>
 
int RECV_PIN = 2;
IRrecv irrecv(RECV_PIN);
 
decode_results results;
 
void setup()
{
        Serial.begin(9600);
 
    // Start the receiver
    irrecv.enableIRIn(); 
 
}
 
void dump(decode_results *results) {
    // # of bytes in the response stream
    int count = results->rawlen;
    // We start at the 3rd byte since the first two bytes are the initial header.
    // Byte 0 = ?
    // Byte 1 = 3200
    // Byte 2 = 1600;
 
    // There is a pair of points for every value - hence why we use the += 2
    for (int i = 3; i < count; i += 2) {
 
        // There is a pair of data points , the first is the delay and the 2nd the value
        int delay = results->rawbuf[i]*USECPERTICK;
 
        // If the value is 1200 it is a binary 1 otherwise a binary 0
        if (results->rawbuf[i+1]*USECPERTICK == 1200)
            Serial.print("1");
        else
            Serial.print("0");
    }
    Serial.println("");
}
 
 
void loop() {
    // Is there any data for us to display?
    if (irrecv.decode(&results)) {
        dump(&results);
        // Continue
        irrecv.resume(); 
    }
}
