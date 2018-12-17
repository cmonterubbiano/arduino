
void setup()
{
  //led Rosso
  pinMode(5, OUTPUT);
  //led verde
  pinMode(6, OUTPUT);
  //led blu
  pinMode(3, OUTPUT);
 
  //init seriale
  Serial.begin(9600);
}
 
void loop() {
  //se arrivano dati...
  if (Serial.available())
  {
    //leggi il carattere in arrivo
    byte data = Serial.read();
 
    if(data == 'q') //accende il led blu
      digitalWrite(3, HIGH);
    else if (data == 'w') //spegne il led blu
      digitalWrite(3, LOW);
 
    if(data == 'a') //accende il led verde
      digitalWrite(5, HIGH);
    else if (data == 's') //spegne il led verde
      digitalWrite(5, LOW);
 
    if(data == 'z') //accende il led rosso
      digitalWrite(6, HIGH);
    else if (data == 'x') //spegne il led rosso
      digitalWrite(6, LOW);
  }
}
