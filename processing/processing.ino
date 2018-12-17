int sensorPin = 0; // Seleziona pin di input
int val = 0;
void setup() {
Serial.begin(9600); // Apri porta seriale
}
void loop() {
val = analogRead(sensorPin) / 4; // Leggi i valori dal sensore
Serial.write(val); // Stampa la variabile nella porta seriale
delay(100); // Aspetta 100 millisecondi
}
