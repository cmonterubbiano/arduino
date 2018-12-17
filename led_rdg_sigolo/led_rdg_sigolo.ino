int a = 1000; //tempo millisecondi di //mantenimento colore
int red = 10; //pin per il led rosso
int green = 12; //pin per il led verde 
int blue = 13; //pin per il led blu 
void setup() { //impostazione pin come uscita 
  pinMode(red, OUTPUT); 
  pinMode(green, OUTPUT); 
  pinMode(blue, OUTPUT); 
} 
void loop() { //ciclo loop 
  analogWrite(blue, random(255)); //la funzione random genera dei numeri casuali in modo da creare dei colori casuali 
  analogWrite(red, random(255)); 
  analogWrite(green, random(255));   
  delay(a); 
}

//Per saperne di più: http://www.iprog.it/blog/arduino/pilotiamo-un-led-rgb-con-arduino/ | iProg
