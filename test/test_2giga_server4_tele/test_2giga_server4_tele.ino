#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
#define MIRF_PAYLOAD 32

byte rxData[MIRF_PAYLOAD + 1];
byte txData[MIRF_PAYLOAD + 1];
char msg[MIRF_PAYLOAD + 1];
int msgIndex = 0;
unsigned  long  time1;

#define  CHISONO  2

/*
 * 4294967295 valore max di un long unsiged
 * tracciato comandi
 * M = millisecondo x riconoscere il comando e renderlo unico
 * V = invio = 0, feedback =1 x comandi normali invio = 3, feedback = 4, x comandi ricevuti da 433
 * R = 1 numero che identifica quante volte il comando e' stato ripetuto
 * C = 1 numero che identifica il comando de eseguire/eseguito
 * I = 1 numero che identifica l'arduino di competenza 0 = raspberry
 * 
 * il tutto in un long unsigned secondo la formula seguente
 * 
 * Valore = (M * 10000) + (V * 1000) + (R * 100) + (C * 10) + I
 * 
 */
#define  NE  50
 // buffer comandi rimbalzati
unsigned  long rcomm[NE];  // comando

unsigned  long  diretto;  //  millisecondi comando diretto

// tempo di attessa dopo cui ripete il comando
#define ATTESA  1000

unsigned long pacchetto;
byte  scheda;  

void setup()
{
  int ix;

  for (ix =0; ix <NE; rcomm[ix++] =0L);
  
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

void  scala_buffer() // cancella dal buffer il comando piu' vecchio
{
  int ix;

  for (ix =0; ix < (NE -1); rcomm[ix] =rcomm[(ix +1)], ix++);
  rcomm[ix] =0L;
}

void  send_pacchetto()
{
   Mirf.setTADDR((byte*)"clie1");
   sprintf(msg, "%010ld", pacchetto);
   memcpy(txData, msg, sizeof(msg));

   Mirf.send(txData);
   while(Mirf.isSending())
   {
   }
   Serial.print("- Ho spedito : ");
   Serial.println(pacchetto);
   delay(50);
}

void  esegui_comando(int  com)    // comando da esguire raggi infrarossi
{
  Serial.print("- Eseguo comando : ");
  Serial.println(com);
  switch  (com)
  {
    
  }
  
}

void  cavoli_miei()
{
  int ix;

  for (ix =0; ix <NE; ix++) //  controllo esistenza comando
  {
    if ((rcomm[ix] /10000L) ==(pacchetto /10000L))
      break;
  }
  if (ix ==NE)      // comando nuovo
  {
    long  wfeed =((pacchetto /1000L) %10L);
    long  wtag =(pacchetto /10000L);
    long  wfin =(pacchetto %1000L);

    if (rcomm[(NE -1)]) //  se il buffer e' pieno
      scala_buffer();
    for (ix =0; rcomm[ix]; ix++); // cerca prima spazio libero sul buffer

    rcomm[ix] =pacchetto;
    esegui_comando((pacchetto /10L %10L));
  //cambia risposta
    if (wfeed ==3)
        diretto =millis();
    else
        pacchetto =(wtag *10000L + ( 1 * 1000L) + wfin);
    send_pacchetto();   //  ripete/rimbalza comando
  }
  else
  {
    long  wfeed =((pacchetto /1000L) %10L);
    long  wtag =(rcomm[ix] /10000L);
    long  wfin =(pacchetto %1000L);

    if (wfeed)    //  ritorno del feedback di una segnalazione diretta all'arduino tramirte 433
    {
      if (wfeed ==4)
      {
        rcomm[ix] =(wtag *10000L + ( 1 * 1000L) + wfin);
        diretto =0L;  // azzero la variabile che conteneva il tempo di trasmissione e bloccava comandi diretti al 433
      }
    }
    else
    {
      pacchetto =(wtag *10000L + ( 1 * 1000L) + wfin);
      send_pacchetto();   //  ripete/rimbalza comando 
    }
  }
}

void  controllo_pacchetto() // analizzo comando
{
  int ix;

  for (ix =0; ix <NE; ix++) //  controllo esistenza comando
  {
    if (rcomm[ix] ==pacchetto)
      break;
  }

  if (ix ==NE)      // comando nuovo
  {
    if ((pacchetto %10L) == CHISONO)
      cavoli_miei();
    else
    {
      if (rcomm[(NE -1)]) //  se il buffer e' pieno
        scala_buffer();
      for (ix =0; rcomm[ix]; ix++); // cerca prima spazio libero sul buffer
      rcomm[ix] =pacchetto;
      send_pacchetto();   //  ripete/rimbalza comando
    }
  }
}

void loop()
{
  int  traok =0;
  int ix;

  *msg ='\0';

  if (Serial.available() > 0)
  {
    int  datoletto;
    unsigned long appot;
    datoletto = Serial.read();
    time1 =millis();
    switch (datoletto)
    {
      case '0':
              sprintf(msg, "%06ld%d%d%d%d", 10L, 0, 0, 1, 1);
              break;
      case '1':
              sprintf(msg, "%06ld%d%d%d%d", 11L, 0, 0, 2, 1);
              break;
      case '2':
              sprintf(msg, "%06ld%d%d%d%d", 11L, 0, 1, 2, 1);
              break;
      case '3':
              sprintf(msg, "%06ld%d%d%d%d", 13L, 0, 0, 4, 1);
              break;
      case '4':
              appot =(time1 *10000L /10000L);
              sprintf(msg, "%06ld%d%d%d%d", appot, 0, 0, 5, 1);
              break;
      case '5':
              sprintf(msg, "%06ld%d%d%d%d", 5L, 4, 0, 1, 1);
              break;
      case '6':
              sprintf(msg, "%06ld%d%d%d%d", 16L, 0, 0, 1, 4);
              break;
      case '7':
              sprintf(msg, "%06ld%d%d%d%d", 17L, 0, 0, 1, 5);
              break;
      case '8':
              sprintf(msg, "%06ld%d%d%d%d", 18L, 0, 0, 1, 6);
              break;
      case '9':
              appot =(time1 *10000L /10000L);
              sprintf(msg, "%06ld%d%d%d%d", appot, 0, 0, 1, 7);
              break;
    }
  }

  //Serial.println(msg);
    pacchetto =atol(msg);
    
  if (strlen(msg) ==10)
    controllo_pacchetto();

  if(!Mirf.isSending() && Mirf.dataReady())
  {
    Mirf.getData(rxData);
    Serial.print("- Dati ricevuti extra : ");
    Serial.println((const char*)rxData);
    *msg ='\0';
    if (strlen((const char*)rxData) ==10)
    {
      strcpy(msg, (const char*)rxData);
      pacchetto =atol(msg);
      controllo_pacchetto();
    }
  }
  // aggiungere controllo di una eventuale ripetizione della avvenuta esecuzione diretta 433
}


