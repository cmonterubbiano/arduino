#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
#define MIRF_PAYLOAD 32

byte rxData[MIRF_PAYLOAD + 1];
byte txData[MIRF_PAYLOAD + 1];
char msg[MIRF_PAYLOAD + 1];
int msgIndex = 0;
unsigned long time1;
int  ntrastot =0;
int  ntimetot =0;
int  npacktot =0;

#define  CHISONO  1

/*
 * 4294967295 valore max di un long unsiged
 * tracciato comandi
 * M = millisecondo x riconoscere il comando e renderlo unico
 * V = comando da eseguire = 0, comando eseguito =1
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

#define  NA  10
 // buffer comandi diretti alla scheda
unsigned  long mcomm[NA];  // comando
unsigned  long memis[NA];  // millisecondo di ricezione/riinvio

// tempo di attessa dopo cui ripete il comando
#define ATTESA  1000

void setup()
{
  int ix;

  for (ix =0; ix <NE; rcomm[ix++] =0L);
  for (ix =0; ix <NA; mcomm[ix] =memis[ix++] =0L);
  
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
  int ix;
  
  /* controllo dei comandi della scheda
   *  controllo se ci sono dei comandi da cancellare perche' terminati o perche' non eseguiti
   *  o dei comandi da ripetere perche' non e' arrivata la risposta
   */
  for (ix =0; ix <NA; ix++)
  {
    long  diffe;
    
    if (!nsecon[ix])
      continue;
    if (millis() < nemis[ix])
      diffe =(millis() + (4294967295L -nemis[ix]));
    else
      diffe =(millis() -nemis[ix]);
 
    if ((ntipo[ix] && diffe > ATTESA)      // se il comando e' stato eseguito ed e' passato un tempo accettabile
    || (!ntipo[ix] && diffe > RIFIUTO))   // se il comando non e' stato eseguito ma e' passato troppo tempo
      nsecon[ix] =0L;                     // cancello dal buffer il comando
  }
  if (Serial.available() > 0)
  {
    int  datoletto;
    int  ntras =0;
    char  appoc[11];
    char  appot[2];

    *msg ='\0';
    datoletto = Serial.read();
    time1 =millis();
    switch (datoletto)
    {
      case '0':
              sprintf(msg, "%010ld%d%06ld", 0L, 0, 0L);
              break;
      case '1':
              sprintf(msg, "%010ld%d%06ld", 1L, 0, 1L);
              break;
      case '2':
              sprintf(msg, "%010ld%d%06ld", 2L, 0, 2L);
              break;
      case '3':
              sprintf(msg, "%010ld%d%06ld", 3L, 0, 3L);
              break;
      case '4':
              sprintf(msg, "%010ld%d%06ld", 4L, 0, 4L);
              break;
      case '5':
              sprintf(msg, "%010ld%d%06ld", 5L, 1, 5L);
              break;
      case '6':
              sprintf(msg, "%010ld%d%06ld", 6L, 1, 6L);
              break;
      case '7':
              sprintf(msg, "%010ld%d%06ld", 7L, 1, 7L);
              break;
      case '8':
              sprintf(msg, "%010ld%d%06ld", 8L, 1, 8L);
              break;
      case '9':
              sprintf(msg, "%010ld%d%06ld", 9L, 1, 9L);
              break;
    }
  //Serial.println(msg);
    sprintf(appoc, "%-10.10s", msg);
    sprintf(appot, "%-1.1s", msg[10]);
    for (ix =0; ix <NE; ix++)
    {
       if (atol(appoc) ==cmills[ix] && atoi(appot) ==tcoman[ix])
          break;
    }
    if (ix ==NE)
    {
       for (ix =0; ix <NE; ix++)
      {
        if (atol(appoc) !=cmills[ix])
          continue;
          
        tcoman[ix] =atoi(appot);
        tmills[ix] =millis();
        break;
      }
      if (ix ==NE)
          for (ix =0; ix <NE; ix++)
      {
          if (tmills[ix])
            continue;
       
          cmills[ix] =atol(appoc);
          tcoman[ix] =atoi(appot);
          tmills[ix] =millis();
          break;
      }
      
   // se supero NE comandi ivio ma non memorizzo
      
      Mirf.setTADDR((byte*)"clie1");
      memcpy(txData, msg, strlen(msg));
    }
    else  traok =1;   // non esegue la trasmissione
    
    while(!traok)
    {
      char  appo[11];
    
      npacktot++;
      Mirf.send(txData);
      while(Mirf.isSending())
      {
      }
      delay(10);
      traok =1;
    }
  }
  memset(rxData, 0, sizeof(rxData));
  if(!Mirf.isSending() && Mirf.dataReady())
  {
    Mirf.getData(rxData);
    Serial.print("- Dati ricevuti extra : ");
    Serial.println((const char*)rxData);
    *msg ='\0';
    if (strlen((const char*)rxData) ==17)
      strcpy(msg, (const char*)rxData);
    else  Serial.println("- Dati ricevuti errati- ");
    if (*msg)
    {
    }
//    Mirf.setTADDR((byte*)"clie1");
//    for (unsigned i = 0; i != MIRF_PAYLOAD; ++i)
//    {
//      txData[i] = rxData[i];
//    }
//    Mirf.send(txData);
  }
  delay(100);
}

void esegui_comando(int  indice)
{
  sprintf(msg, "%010ld%d%d%06ld", nsecond[indice], ntipo[ix], 
  Mirf.setTADDR((byte*)"clie1");
  memcpy(txData, msg, strlen(msg));
  Mirf.send(txData);
  while(Mirf.isSending())
  {
  }
  delay(10);
}


