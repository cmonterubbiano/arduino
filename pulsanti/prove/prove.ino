char incomingByte = 0;	// for incoming serial data
int  ix =0;
unsigned long  ultimo;

void setup() {
	Serial.begin(9600);	// opens serial port, sets data rate to 9600 bps
}

void loop() 
{
	// send data only when you receive data:
	if (Serial.available())
        {
		// read the incoming byte:
		incomingByte = Serial.read();

		// say what you got:
//		Serial.print("I received: ");
		Serial.print(incomingByte);
                ix++;
                ultimo =millis();
	}
        else  if (ix && (millis() -ultimo) >20)
        {
          Serial.println("");
          ix =0;
        }
}
