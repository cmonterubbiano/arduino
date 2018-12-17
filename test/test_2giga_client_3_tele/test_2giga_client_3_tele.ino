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

#define  CHISONO  1

/*
 * 4294967295 valore max di un long unsiged
 * tracciato comandi
 * M = millisecondo x riconoscere il comando e renderlo unico
 * V = invio = 0, feedback =1
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

unsigned long pacchetto;
byte  scheda;  

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

void  scala_buffer(int  flag) // cancella dal buffer il comando piu' vecchio
{
  int ix;

  if (flag)
  {
    for (ix =0; ix < (NE -1); mcomm[ix] =mcomm[(ix +1)], ix++);
    mcomm[ix] =0L;
  }
  else
  {
    for (ix =0; ix < (NA -1); rcomm[ix] =rcomm[(ix +1)], memis[ix] =memis[(ix +1)], ix++);
    rcomm[ix] =memis[ix] =0L;
  }
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
   delay(10);
}

void  esegui_comando(int  com)    // comando da esguire raggi infrarossi
{
  switch  (com)
  {
    
  }
  
}

void  cavoli_miei()
{
  int ix;

  for (ix =0; ix <NA; ix++) //  controllo esistenza comando
  {
    if ((mcomm[ix] /10000L) ==(pacchetto /10000L))
      break;
  }
  if (ix ==NA)      // comando nuovo
  {
    if (mcomm[(NA -1)]) //  se il buffer e' pieno
      scala_buffer(0);
    for (ix =0; mcomm[ix]; ix++); // cerca prima spazio libero sul buffer
     //cambia risposta
    mcomm[ix] =pacchetto;
    esegui_comando((pacchetto /10L %10L));
    send_pacchetto();   //  ripete/rimbalza comando
  }
  else
  {
      
  }
}

void  cavoli_altrui() // comandi da rimbalzare se nuovi
{
  int ix;

  for (ix =0; ix <NE; ix++) //  controllo esistenza comando
  {
    if (rcomm[ix] ==pacchetto)
      break;
  }
  if (ix ==NE)      // comando nuovo
  {
    if (mcomm[(NE -1)]) //  se il buffer e' pieno
      scala_buffer(1);
    for (ix =0; rcomm[ix]; ix++); // cerca prima spazio libero sul buffer
    rcomm[ix] =pacchetto;
    send_pacchetto();   //  ripete/rimbalza comando
  }
}

void loop()
{
  int  traok =0;
  int ix;
  
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
              sprintf(msg, "%06ld%d%d%d%d", 0L, 0, 0, 1, 2);
              break;
      case '1':
              sprintf(msg, "%06ld%d%d%d%d", 1L, 0, 0, 2, 2);
              break;
      case '2':
              sprintf(msg, "%06ld%d%d%d%d", 2L, 0, 0, 3, 2);
              break;
      case '3':
              sprintf(msg, "%06ld%d%d%d%d", 3L, 0, 0, 4, 2);
              break;
      case '4':
              sprintf(msg, "%06ld%d%d%d%d", 4L, 0, 0, 5, 2);
              break;
      case '5':
              sprintf(msg, "%06ld%d%d%d%d", 5L, 0, 0, 1, 3);
              break;
      case '6':
              sprintf(msg, "%06ld%d%d%d%d", 6L, 0, 0, 1, 4);
              break;
      case '7':
              sprintf(msg, "%06ld%d%d%d%d", 7L, 0, 0, 1, 5);
              break;
      case '8':
              sprintf(msg, "%06ld%d%d%d%d", 8L, 0, 0, 1, 6);
              break;
      case '9':
              sprintf(msg, "%06ld%d%d%d%d", 9L, 0, 0, 1, 7);
              break;
    }
  //Serial.println(msg);
    pacchetto =atol(msg);
    scheda =(pacchetto %10L);
    
    if (scheda ==CHISONO)
      cavoli_miei();
    else
      cavoli_altrui();
      
//    for (ix =0; ix <NE; ix++)
//    {
//       if (atol(appoc) ==cmills[ix] && atoi(appot) ==tcoman[ix])
//          break;
//    }
//    if (ix ==NE)
//    {
//       for (ix =0; ix <NE; ix++)
//      {
//        if (atol(appoc) !=cmills[ix])
//          continue;
//          
//        tcoman[ix] =atoi(appot);
//        tmills[ix] =millis();
//        break;
//      }
//      if (ix ==NE)
//          for (ix =0; ix <NE; ix++)
//      {
//          if (tmills[ix])
//            continue;
//       
//          cmills[ix] =atol(appoc);
//          tcoman[ix] =atoi(appot);
//          tmills[ix] =millis();
//          break;
//      }
//      
//   // se supero NE comandi ivio ma non memorizzo
//      
//      Mirf.setTADDR((byte*)"clie1");
//      memcpy(txData, msg, strlen(msg));
//    }
//    else  traok =1;   // non esegue la trasmissione
//    
//    while(!traok)
//    {
//      char  appo[11];
//    
//      npacktot++;
//      Mirf.send(txData);
//      while(Mirf.isSending())
//      {
//      }
//      delay(10);
//      traok =1;
//    }
//  }
//  memset(rxData, 0, sizeof(rxData));
//  if(!Mirf.isSending() && Mirf.dataReady())
//  {
//    Mirf.getData(rxData);
//    Serial.print("- Dati ricevuti extra : ");
//    Serial.println((const char*)rxData);
//    *msg ='\0';
//    if (strlen((const char*)rxData) ==17)
//      strcpy(msg, (const char*)rxData);
//    else  Serial.println("- Dati ricevuti errati- ");
//    if (*msg)
//    {
//    }
////    Mirf.setTADDR((byte*)"clie1");
////    for (unsigned i = 0; i != MIRF_PAYLOAD; ++i)
////    {
////      txData[i] = rxData[i];
////    }
////    Mirf.send(txData);
//  }
//  delay(100);
}


//void esegui_comando(int  indice)
//{
//  sprintf(msg, "%010ld%d%d%06ld", nsecond[indice], ntipo[ix], 
//  Mirf.setTADDR((byte*)"clie1");
//  memcpy(txData, msg, strlen(msg));
//  Mirf.send(txData);
//  while(Mirf.isSending())
//  {
//  }
//  delay(10);
}


