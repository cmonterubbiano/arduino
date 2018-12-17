/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/
int redPin =11;  // LED stato generale
int grePin =10;  // LED stato generale
int bluPin =6;  // LED stato generale

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(redPin, OUTPUT);
  pinMode(grePin, OUTPUT);
  pinMode(bluPin, OUTPUT);
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  Serial.println("Rosso");
  digitalWrite(redPin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(3000);                       // wait for a second
  Serial.println("Rosso_metà");
  analogWrite(redPin, 32);   // turn the LED on (HIGH is the voltage level)
  delay(3000);                       // wait for a second
  digitalWrite(redPin, LOW);    // turn the LED off by making the voltage LOW
  Serial.println("Verde");
  digitalWrite(grePin, HIGH);
  delay(3000);                       // wait for a second
  Serial.println("Verde_32");
  analogWrite(grePin, 32);
  delay(3000);                       // wait for a second
  digitalWrite(grePin, LOW);    // turn the LED off by making the voltage LOW
  Serial.println("Blu");
  digitalWrite(bluPin, HIGH);
  delay(3000);                       // wait for a second
  digitalWrite(bluPin, LOW);    // turn the LED off by making the voltage LOW
  Serial.println("Rosso_Verde");
  digitalWrite(redPin, HIGH);
  digitalWrite(grePin, HIGH);
  delay(3000);                       // wait for a second
  digitalWrite(redPin, LOW);    // turn the LED off by making the voltage LOW
  Serial.println("Verde_Blu");
  digitalWrite(bluPin, HIGH);
  delay(3000);                       // wait for a second
  digitalWrite(grePin, LOW);    // turn the LED off by making the voltage LOW
  Serial.println("Rosso_Blu");
  digitalWrite(redPin, HIGH);
  delay(3000);
  Serial.println("Rosso_Verde_Blu");
  digitalWrite(grePin, HIGH);
  delay(3000);
  digitalWrite(redPin, LOW);    // turn the LED off by making the voltage LOW// wait for a second
  digitalWrite(grePin, LOW);    // turn the LED off by making the voltage LOW// wait for a second
  digitalWrite(bluPin, LOW);    // turn the LED off by making the voltage LOW// wait for a second
}
