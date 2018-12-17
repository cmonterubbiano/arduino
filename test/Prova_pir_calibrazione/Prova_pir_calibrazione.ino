/* Programam che utilizza il sensore di presenza PIR HC-SR501, per rilevare la presenza di un oggetto in movimento e accende un LED, collegato al PIN 13*/
 
// Tempo di calibrazione del sensore
int calibrationTime = 30;        
 
//Il tempo in cui l'uscita sia bassa
long unsigned int lowIn;         
 
// valore di millisecondi, per cui si ritiene che ci sia "quiete"
long unsigned int pause = 5000;  
 
boolean lockLow = true;
boolean takeLowTime;  
 
int pirPin = 7;    //il PIN di Arduino a cui è collegato il sensore
int ledPin = 13;   //il PIN a cui è connesso il LED
int pinSpeaker= 10;
 
// Impostazione del sensore 
void setup(){
  Serial.begin(9600);
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(pinSpeaker, OUTPUT);
  digitalWrite(pirPin, LOW);
 
  //Fase di calibrazione
  Serial.print("calibrating sensor ");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(1000);
      }
    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
    delay(50);
  }
 
void loop()
  {
 
    // Questo IF permette di stabilre se il sensore rileva un oggetto in movimento
    if(digitalRead(pirPin) == HIGH)
    {
      digitalWrite(ledPin, HIGH);   //Accendiamo il LED
      if(lockLow)
      { 
        lockLow = false;            
        Serial.println("---");
        Serial.print("motion detected at ");
        Serial.print(millis()/1000);
        Serial.println(" sec"); 
        tone(10, 700, 100);
        delay(50);
      }         
      takeLowTime = true;
    }
    // Questo IF permette di stabilire se non c'è più nessun movimento
    if(digitalRead(pirPin) == LOW)
    {       
      digitalWrite(ledPin, LOW);  //Si spegne il LED
 
      if(takeLowTime)
      {
        lowIn = millis();          
        takeLowTime = false;       
      }
      if(!lockLow && millis() - lowIn > pause)
      {  
        lockLow = true;                        
        Serial.print("motion ended at ");      //output
        Serial.print((millis() - pause)/1000);
        Serial.println(" sec");
        delay(50);
      }
    }
  }
  
  void tone(long duration, int freq)
  {
    duration *= 1000;
    int period = (1.0 / freq) * 1000000;
    long elapsed_time = 0;
    while (elapsed_time < duration)
    {
      digitalWrite(pinSpeaker,HIGH);
      delayMicroseconds(period / 2);
      digitalWrite(pinSpeaker, LOW);
      delayMicroseconds(period / 2);
      elapsed_time += (period);
    }
  }  

