

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();               
#include <OneWire.h>

OneWire  ds(9);  // on pin 9 (a 4.7K resistor is necessary)
byte addr[8] = {0x28,0xF5,0xA8,0xC0,0x03,0x00,0x00,0x57};     

long  intervallo =20000;    //tempo in millesencodi x l'invio dei dati

int analogInPin = A0;
int sensorValue = 0;
unsigned long time;
int ultval =0;
int soglia =250;
unsigned long intervalloImpulsi = 0;
long letturaAttualeCorrente = 0;
int contatoreWattOra = 0;
unsigned long ultimaChiamataInvio = 0;
int  flag =0;
long letturaFormattata;
long ultimoinvio = 0;
int  flagt =0;

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
    
  if((millis() - ultimaChiamataInvio) >= (intervallo /2) && !flagt)
  {
    leggiTemperatura();
    flagt =1;
  }
  
  if((millis() - ultimaChiamataInvio) >= (intervallo -2000) && !flag)
  {
    ds.reset();
    ds.select(addr);
    ds.write(0x44, 1);        // start conversion, with parasite power on at the end
    flag =1;
  }
  // invio ad ogni "intervallo
  if((millis() - ultimaChiamataInvio) >= intervallo)
  {
    ultimaChiamataInvio = millis();
    if (!letturaAttualeCorrente)
    {
      intervalloImpulsi = millis() - time;
      // se nell'intervallo non ci sono state letture
      // se non sono passati 6 minuti ripeto l'ultimo invio
      if (intervalloImpulsi >600000)
        letturaAttualeCorrente = 3600000 / intervalloImpulsi;
      else
        letturaAttualeCorrente =ultimoinvio; 
    }
    inviaSegnaleVia433(letturaAttualeCorrente);
    ultimoinvio =letturaAttualeCorrente;
    letturaAttualeCorrente =0;
    leggiTemperatura();
    flag = flagt =0;
  }
  delay(10);
}

void inviaSegnaleVia433(long letturaWatt)
{
  // formatto il segnale per essre riconosciuto come lettura contatore:
  letturaFormattata = formattaValore(7, letturaWatt);
  
  // ivio 433
  Serial.print("----------------invio 433: ");
  Serial.print(letturaWatt);
  Serial.print(" - ");
  Serial.println(letturaFormattata);
  mySwitch.send(letturaFormattata, 24);  
}

void  leggiTemperatura()
{
  byte i;
  byte data[12];
  float celsius, fahrenheit;
  
//  ds.reset();
//  ds.select(addr);
//  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
//  
//  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 9; i++) 
    data[i] = ds.read();

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];

  byte cfg = (data[4] & 0x60);
  // at lower res, the low bits are undefined, so let's zero them
  if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
  else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
  else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
  //// default is 12 bit resolution, 750 ms conversion time

  celsius = (float)raw / 16.0;
  
  Serial.print("Temperatura = ");
  Serial.print(celsius);
  Serial.println(" Celsius, ");
  letturaFormattata = formattaValore((celsius <.0 ? 5 : 6), (celsius *(celsius <.0 ? -100 : 100)));
  Serial.print("----------------invio 433: ");
  Serial.print(celsius);
  Serial.print(" - ");
  Serial.println(letturaFormattata);
  mySwitch.send(letturaFormattata, 24);
}

long formattaValore(int  tipo, long letturaWatt)
{
  long  appo = letturaWatt /1000;
  long  risultato;
  appo +=(letturaWatt /100 %10);
  appo +=(letturaWatt /10 %10);
  appo +=(letturaWatt /1 %10);
  appo +=tipo;
  appo = appo %10;

  risultato =(tipo * 100000) +(letturaWatt * 10) +appo;
  return(risultato);
}



