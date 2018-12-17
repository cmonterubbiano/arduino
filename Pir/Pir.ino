/*
 * Allarme volumetrico pin 7 
 * con cicalino passivi pin 8
 * chiamata web pin 10-11-12-13, con servizio IFTTT 
 */
 

//Henry's Bench
// HC-SR501 Motion Detector
// Sample Sketch

int ledPin = 13;  // LED on Pin 13 of Arduino
int pirPin = 7; // Input for HC-S501
int buzzer =8;  // Set the control the buzzer digital IO pin

int pirValue; // Place to store read PIR Value
int stato;

void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);
  pinMode(buzzer, OUTPUT);
 
  digitalWrite(ledPin, LOW);
  Serial.begin(9600);
  Serial.println("Pronto");
}

void loop()
{
  pirValue = digitalRead(pirPin);
  digitalWrite(ledPin, pirValue);
  if (stato != pirValue)
  {
    stato =pirValue;
    if (pirValue)
    {
      int i;
 
      Serial.println(F("Acceso"));
      for(i=0; i<800; i++)// Wen a frequency sound
      {
        digitalWrite(buzzer,HIGH);// Sound
        delay(1);//delay 1ms
        digitalWrite(buzzer,LOW);//Not sound
        delay(1);// delay 1ms
      }
  }
  else
      Serial.println(F("Spento"));
  }
}
