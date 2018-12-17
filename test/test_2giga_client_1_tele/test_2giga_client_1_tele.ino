#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
#define MIRF_PAYLOAD 32

byte rxData[MIRF_PAYLOAD + 1];
byte txData[MIRF_PAYLOAD + 1];
char msg[100];
int msgIndex = 0;
unsigned long time1;
int  ntrastot =0;
int  ntimetot =0;
int  npacktot =0;

void setup()
{
  pinMode(9, OUTPUT);
  memset(rxData, 0, sizeof(rxData));
  memset(txData, 0, sizeof(txData));
  Serial.begin(9600);
  Mirf.cePin = 8;
  Mirf.csnPin = 7;
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte*)"clie1");
  Mirf.payload = MIRF_PAYLOAD;
  Mirf.channel = 10;
  Mirf.config();
}

void loop()
{
  int  traok =0;
 
  if (Serial.available() > 0)
  {
    int  datoletto;
    int  ntras =0;
    
    datoletto = Serial.read();
    time1 =millis();
    if (datoletto =='A')
      sprintf(msg, "%010ld%s", time1, "Segnale1");
    else
      sprintf(msg, "%010ld%s", time1, "Segnale2");
  //Serial.println(msg);
    Mirf.setTADDR((byte*)"serv1");
    memcpy(txData, msg, strlen(msg));
    while(!traok && ntras <50)
    {
      char  appo[11];
    
      npacktot++;
      Mirf.send(txData);
      while(Mirf.isSending())
      {
      }
      delay(10);
      unsigned long time = millis();
      while(!Mirf.dataReady())
      {
        if (( millis() - time ) > 1000)
        {
          Serial.println("timeout");
          ntimetot++;
          break;
        }  
      }
      Mirf.getData(rxData);
      sprintf(appo, "%-10.10s", rxData);
      if (atol(appo) ==time1)
        traok =1;
      else
        ntrastot++;
      ntras++;
      //Serial.println((const char*)rxData);
      Serial.print(traok);
      Serial.print(" - trasmissioni pacchetto : ");
      Serial.print(ntras);
      Serial.print(" - pacchetti trasmessi : ");
      Serial.print(npacktot);
      Serial.print(" - ripetizioni trasmissioni : ");
      Serial.print(ntrastot);
      Serial.print(" - Numero timeout totali : ");
      Serial.println(ntimetot);
      if (traok)
     { 
       digitalWrite(9, HIGH);   // turn the LED on (HIGH is the voltage level)
       delay(1);              // wait for a second
       digitalWrite(9, LOW);    // turn the LED off by making the voltage LOW
     }    
    }
  }
  memset(rxData, 0, sizeof(rxData));
  if(!Mirf.isSending() && Mirf.dataReady())
  {
    Mirf.getData(rxData);
    Serial.print("- Dati ricevuti extra : ");
    Serial.println((const char*)rxData);
    Mirf.setTADDR((byte*)"clie1");
    for (unsigned i = 0; i != MIRF_PAYLOAD; ++i)
    {
      txData[i] = rxData[i];
    }
    Mirf.send(txData);
  }
  delay(100);
}
