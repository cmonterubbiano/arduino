/*
  Serial Event example
 
 When new serial data arrives, this sketch adds it to a String.
 When a newline is received, the loop prints the string and 
 clears it.
 
 A good test for this is to try it with a GPS receiver 
 that sends out NMEA 0183 sentences. 
 
 Created 9 May 2011
 by Tom Igoe
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/SerialEvent
 nulla serialEvent1 () { 
} 

nulla serialEvent2 () {  
} 

nulla serialEvent3 () { 
 
Arduino Mega only: 
Serial1.begin(speed) 
Serial2.begin(speed) 
Serial3.begin(speed) 
Serial1.begin(speed, config) 
Serial2.begin(speed, config) 
Serial3.begin(speed, config)
}
 */

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
String inputString1 = "";         // a string to hold incoming data
boolean stringComplete1 = false;  // whether the string is complete

void setup()
{
  // initialize serial:
  Serial.begin(9600);
  Serial1.begin(9600);
  // reserve 200 bytes for the inputString:
  Serial.println("ATT. :Per terminare la stringa adopera Alt 126 ossia ~");
  Serial.println("non riesco ad intercettare ne LF ne RETURN da tastiera");
  Serial.println("------------------------------------------------------");
  inputString.reserve(200);
  inputString1.reserve(200);
  inputString = "";
  inputString1 = "";
}

void loop()
{
  // print the string when a newline arrives:
  if (stringComplete)
  {
    Serial.print("trasmessa - ");
    Serial.println(inputString);
    Serial1.print("Prova di trasmissine tra due arduino MEGA\n");
    inputString = "";
    stringComplete = false;
  }
  if (stringComplete1)
  {
    Serial.print("arrivata - ");
    Serial.println(inputString1); 
    inputString1 = "";
    stringComplete1 = false;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent()
{
  while (Serial.available())
  {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:

    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '~')
      stringComplete = true;
    else
      inputString += inChar;
  } 
}

void serialEvent1()
{
  while (Serial1.available())
  {
    char inChar = (char)Serial1.read();
    
    if (inChar == '\n')
      stringComplete1 = true;
    else
      inputString1 += inChar;
  } 
}
