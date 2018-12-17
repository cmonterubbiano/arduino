int ix =0;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

Serial.print("Ciao Pi :\t");
Serial.println(ix++);
  delay(2000);
}
