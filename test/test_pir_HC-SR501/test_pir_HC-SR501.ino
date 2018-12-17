/*  Programma:test_sensore_pir.ino 
Il programma accende un led quando un corpo caldo
è in movimento.
di Adriano Gandolfo  https://www.adrirobot.it
*/

//Definizione dei PIN
int pirPin = 2;    //il PIN di Arduino a cui è collegato il sensore
int ledPin = 13;   //il PIN a cui è connesso il LED

// Tempo di calibrazione del sensore
int calibrationTime = 30;

//Il tempo in cui l'uscita sia bassa
long unsigned int lowIn;

// valore di millisecondi, per cui si ritiene che ci sia "quiete"
long unsigned int pause = 5000;

boolean lockLow = true;
boolean takeLowTime;

// Impostazione del sensore
void setup() {
  Serial.begin(9600);
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(pirPin, LOW);

  //Fase di calibrazione
  Serial.print("Calibrazione del sensore ");
  for (int i = 0; i < calibrationTime; i++) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println(" Fatto");
  Serial.println("SENSORE ATTIVO");
  delay(50);
}

void loop() {

  // Questo IF permette di stabilre se il sensore rileva un oggetto in movimento
  if (digitalRead(pirPin) == HIGH) {
    digitalWrite(ledPin, HIGH);   //Accendiamo il LED
    if (lockLow) {

      lockLow = false;
      Serial.println("---");
      Serial.print("Movimento rilevato a ");
      Serial.print(millis() / 1000);
      Serial.println(" sec");
      delay(50);
    }
    takeLowTime = true;
  }
  // Questo IF permette di stabilire se non c'è più nessun movimento
  if (digitalRead(pirPin) == LOW) {
    digitalWrite(ledPin, LOW);  //Si spegne il LED

    if (takeLowTime) {
      lowIn = millis();
      takeLowTime = false;
    }

    if (!lockLow && millis() - lowIn > pause) {

      lockLow = true;
      Serial.print("Movimento terminato a ");      //output
      Serial.print((millis() - pause) / 1000);
      Serial.println(" sec");
      delay(50);
    }
  }
}
