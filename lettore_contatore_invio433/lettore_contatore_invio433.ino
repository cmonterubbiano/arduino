

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();


 
int analogInPin = A0;
int sensorValue = 0;
unsigned long time;
int ultval =0;
int soglia =250;
unsigned long intervalloImpulsi = 0;
long letturaAttualeCorrente = 0;
int contatoreWattOra = 0;
unsigned long ultimaChiamataInvio = 0;

void setup() 
{
  Serial.begin(9600);
  pinMode( analogInPin, INPUT);
  time = millis();
  
  //Serial.println(formattaValore(2316));
  pinMode(13, OUTPUT);
  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(10);
    // Optional set pulse length.
  mySwitch.setPulseLength(350);
  delay(1000);
  
  //inviaSegnaleVia433(2316);
}
 
void loop()
{
  
  //controllo dell'overflow di millis:
  if(millis() < time)
  {
    time = 0;
    ultimaChiamataInvio = 0;
  }
  
  sensorValue = analogRead(A0);

  // Sono nel caso in cui c'e' un'accensione di LED:
  if (!ultval && sensorValue >=soglia)
  {
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
     
  } 
  else 
  { 
    // Qui il led potrebbe essere SPENTO essere ANCORA acceso, da non considerare
    //Serial.print(sensorValue);
    //Serial.print("-");
  }
  
  // se è sotto la soglia (ri)metto il valore a basso 
  if (sensorValue < soglia)
    ultval=0;
  
  // invio ogni 30 sec
  if((millis() - ultimaChiamataInvio) >= 20000)
  {
    ultimaChiamataInvio = millis();
    inviaSegnaleVia433(letturaAttualeCorrente);
  }
  
  delay(10);
}

void inviaSegnaleVia433(long letturaWatt)
{
  long letturaWattFormattata;
  // formatto il segnale per essre riconosciuto come lettura contatore:
  letturaWattFormattata = formattaValore(letturaWatt);
  
  // ivio 433
  Serial.println("----------------invio 433:: ");
  Serial.print(letturaWatt);
  Serial.print(" - ");
  Serial.print(letturaWattFormattata);
  mySwitch.send(letturaWattFormattata, 24);  
}


long formattaValore(long letturaWatt)
{
  long  appo = letturaWatt /1000;
  long  risultato;
  appo +=(letturaWatt /100 %10);
  appo +=(letturaWatt /10 %10);
  appo +=(letturaWatt /1 %10);
  appo +=7;
  appo = appo %10;

  risultato =700000 +(letturaWatt * 10) +appo;
  return(risultato);
}



