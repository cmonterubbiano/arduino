#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int analogInPin = A0;
int sensorValue = 0;
unsigned long time;
int ultval =0;
int soglia =400;
int sogliasup =600;
unsigned long intervalloImpulsi = 0;
long letturaAttualeCorrente = 0;
int contatoreWattOra = 0;

void setup()
{
  Serial.begin(9600);
  pinMode( analogInPin, INPUT);
  time = millis();
   // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Consumo in Kw/h");
}

void loop()
{
  if(millis() < time)
  {
    time = 0;
  }
  
  sensorValue = analogRead(A0); // Sono nel caso in cui c'e' un'accensione di LED:
  if (!ultval && sensorValue <=soglia)
  {
    char  appo[17];
    Serial.println("");
    ultval =1;
    Serial.print("sensor = " );
    Serial.print(sensorValue);
    Serial.print("- time = " );
    intervalloImpulsi = millis() - time;
    Serial.println(intervalloImpulsi);
    time = millis();

    contatoreWattOra++;
     
    letturaAttualeCorrente = 3600000 / intervalloImpulsi;
    Serial.println(letturaAttualeCorrente);
     lcd.setCursor(0, 1);
  // print the number of seconds since reset:
    lcd.print("                ");
    lcd.setCursor(0, 1);
    sprintf(appo, "%6ld,%03ld", (letturaAttualeCorrente/1000), (letturaAttualeCorrente %1000));
    lcd.print(appo);
  } 
  else 
  { 
    // Qui il led potrebbe essere SPENTO essere ANCORA acceso, da non considerare
    //Serial.print(sensorValue);
    //Serial.print("-");
  }
  
  // se è sotto la soglia (ri)metto il valore a basso 
  if (ultval && sensorValue > sogliasup)
  {
    ultval=0;
        time = millis();
  }
}
