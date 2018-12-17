#include <avr/pgmspace.h>
#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

#include <IRremote.h>
#include <RCSwitch.h>

IRsend irsend;
RCSwitch mySwitch = RCSwitch();

//You could also try 36,40 & 56 frequencies as a test NORMAL = 38
int khz = 38; //NB Change this default value as neccessary to the correct modulation frequency

#define MIRF_PAYLOAD 32

byte rxData[MIRF_PAYLOAD + 1];
byte txData[MIRF_PAYLOAD + 1];

//toshiba accensione
//unsigned int Signal_0_0[] = {4416, 4312, 544, 1604, 552, 1604, 568, 1584, 544, 1604, 544, 532, 548, 528, 572, 1580, 548, 528, 544, 532, 544, 532, 544, 532, 540, 560, 544, 1580, 572, 1584, 540, 532, 544, 1608, 572, 504, 544, 532, 544, 532, 544, 1608, 568, 508, 544, 532, 540, 1608, 544, 1608, 548, 1604, 568, 1584, 544, 1604, 544, 532, 572, 1580, 548, 1604, 568, 508, 568, 508, 544, 528, 548, 532, 540, 532, 548, 528, 548, 532, 568, 504, 544, 536, 564, 1584, 544, 532, 540, 1612, 544, 1608, 568, 508, 540, 536, 544, 528, 572, 504, 548, 528, 548, 528, 548, 1604, 568, 508, 568, 508, 544, 532, 572, 504, 540, 1608, 572, 1584, 540, 532, 544, 532, 544, 532, 544, 532, 540, 536, 540, 532, 548, 528, 548, 532, 568, 508, 568, 508, 540, 1608, 572, 504, 544, 532, 544, 532, 544, 1608, 568, 508, 544, 7428, 4360, 4340, 544, 1608, 568, 1584, 568, 1584, 540, 1608, 544, 532, 548, 528, 544, 1608, 544, 532, 544, 532, 544, 532, 540, 536, 540, 532, 548, 1608, 544, 1608, 564, 508, 572, 1580, 540, 536, 544, 528, 572, 508, 544, 1608, 540, 536, 540, 532, 572, 1580, 548, 1628, 544, 1584, 568, 1584, 540, 1612, 568, 504, 572, 1584, 564, 1608, 548, 504, 572, 504, 572, 504, 540, 536, 568, 504, 572, 508, 568, 508, 544, 532, 540, 536, 540, 1608, 544, 532, 544, 1608, 544, 1608, 544, 556, 520, 556, 520, 556, 544, 508, 540, 532, 544, 532, 544, 1636, 544, 532, 516, 532, 568, 532, 520, 532, 572, 1580, 540, 1612, 568, 504, 572, 504, 572, 508, 568, 508, 540, 536, 540, 532, 544, 532, 540, 564, 512, 536, 544, 528, 572, 1584, 544, 532, 564, 536, 544, 508, 540, 1608, 544, 532, 544};
//toshiba spegnimento
//unsigned int Signal_0_1[] = {4388, 4340, 544, 1604, 572, 1580, 572, 1580, 544, 1608, 568, 508, 544, 532, 544, 1604, 572, 508, 544, 532, 540, 532, 568, 508, 544, 532, 572, 1580, 548, 1604, 544, 532, 544, 1604, 572, 504, 544, 532, 572, 504, 572, 1580, 568, 508, 568, 508, 544, 1604, 576, 1576, 572, 1584, 544, 1604, 544, 1608, 544, 532, 572, 1576, 548, 1608, 544, 532, 564, 508, 572, 504, 544, 532, 572, 504, 548, 524, 548, 532, 544, 532, 544, 532, 568, 1580, 544, 532, 572, 1584, 540, 1608, 544, 532, 572, 504, 572, 504, 568, 504, 572, 508, 568, 508, 568, 1580, 572, 504, 572, 504, 572, 504, 572, 1604, 544, 1584, 572, 1608, 512, 532, 548, 532, 544, 532, 544, 532, 540, 532, 544, 532, 572, 504, 544, 532, 548, 528, 544, 532, 544, 1608, 568, 508, 540, 536, 568, 1580, 572, 1580, 548, 532, 540, 7428, 4364, 4340, 540, 1636, 544, 1580, 548, 1600, 572, 1584, 544, 532, 540, 536, 540, 1608, 572, 504, 572, 532, 516, 536, 564, 508, 568, 508, 572, 1580, 544, 1604, 572, 504, 548, 1608, 540, 536, 540, 556, 544, 508, 544, 1608, 540, 532, 548, 528, 548, 1632, 520, 1608, 568, 1580, 544, 1608, 544, 1608, 568, 508, 544, 1608, 568, 1580, 548, 528, 548, 532, 564, 512, 540, 556, 520, 532, 544, 532, 544, 528, 572, 504, 572, 508, 544, 1608, 540, 532, 544, 1608, 544, 1608, 572, 508, 540, 532, 568, 508, 544, 532, 596, 480, 572, 504, 540, 1608, 572, 504, 572, 508, 544, 532, 564, 1584, 544, 1608, 540, 1612, 544, 532, 544, 532, 544, 532, 544, 556, 540, 512, 540, 532, 572, 504, 544, 532, 544, 536, 564, 508, 568, 1584, 544, 532, 544, 528, 572, 1580, 548, 1604, 544, 532, 572};
unsigned Signal_0_0[] ={4500, 4350, 650, 1550, 600,1550, 650, 1550, 600, 550, 600, 500, 600, 500, 550, 500, 650, 500, 600, 1600, 600, 1550, 600, 1550, 650, 550, 600, 500, 550, 500, 600, 500, 600, 550, 600, 1550, 650, 1550, 600, 1550, 600, 1650, 600, 1600, 550, 500, 650, 450, 600, 550, 600, 500, 600, 450, 650, 450, 600, 550, 650, 450, 600, 1600, 550, 1600, 600, 1600, 600};
unsigned Signal_0_1[] ={4500,4350,600,1600,600,1550,600,1600,550,600,550,550,550,550,550,500,550,600,600,
                  1550,600,1600,600,1550,600,600,550,550,550,500,550,550,550,600,550,500,600,500,600,
                  1600,550,600,550,550,550,500,600,500,550,600,600,1550,600,1600,600,500,550,1700,550,1600,550,1650,550,1600,550,1600,600};

unsigned long timet =0L;
int  segnale =0;
unsigned long timer =0L;
char msg[50];

void setup()
{
  memset(rxData, 0, sizeof(rxData));
  memset(txData, 0, sizeof(txData));
  Serial.begin(9600);
  Mirf.cePin = 8;
  Mirf.csnPin = 7;
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte*)"serv1");
  Mirf.payload = MIRF_PAYLOAD;
  Mirf.channel = 10;
  Mirf.config();
  
  mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
                               // Receiver on interrupt 1 => that is pin #3
  irsend.enableIROut(khz);
}
void loop()
{
  memset(rxData, 0, sizeof(rxData));
  if(!Mirf.isSending() && Mirf.dataReady())
  {
    
    Mirf.getData(rxData);
   // Serial.println((const char*)rxData);
    sprintf(msg, "%-10.10s", (const char*)rxData);
    Serial.println(msg);
    if (atol(msg) !=timer)
    {
      sprintf(msg, "%-8.8s", &rxData[10]);
      Serial.println(msg);
      if (!strcmp(msg, "Segnale1"))
        irsend.sendRaw(Signal_0_0, sizeof(Signal_0_0)/sizeof(int), khz);
      if (!strcmp(msg, "Segnale2"))
        irsend.sendRaw(Signal_0_1, sizeof(Signal_0_1)/sizeof(int),khz);
    }
    timer =atol(msg); 
    Mirf.setTADDR((byte*)"clie1");
    for (unsigned i = 0; i != MIRF_PAYLOAD; ++i)
    {
      txData[i] = rxData[i];
    }
    Mirf.send(txData);
  }
  
  if (mySwitch.available())
  {
    int value = mySwitch.getReceivedValue();
    
    if (value == 0)
      Serial.print("Unknown encoding");
    else
    {
      long  codice =mySwitch.getReceivedValue();
      int  ix;
      
      Serial.print("Received ");
      Serial.print(codice);
      Serial.print(" / ");
      Serial.print( mySwitch.getReceivedBitlength() );
      Serial.print("bit ");
      Serial.print("Protocol: ");
      Serial.println( mySwitch.getReceivedProtocol() );
      if (codice ==12844044L)
      {
        Serial.println("Tasto Accendi");
        irsend.sendRaw(Signal_0_0, sizeof(Signal_0_0)/sizeof(int), khz);
        timet =millis();
        segnale =1;    
      } 
      if (codice ==12844035L)
      {
        Serial.println("Tasto spegni");
        irsend.sendRaw(Signal_0_1, sizeof(Signal_0_1)/sizeof(int),khz);
        timet =millis();
        segnale =2;    
      } 
    }
    mySwitch.resetAvailable();
  }
  if (segnale)
  {
    sprintf(msg, "%010ld%s", timet, (segnale ==1 ? "Segnale1" : "Segnale2"));
    Serial.print("spedisco: ");
    Serial.println(msg);
    Mirf.setTADDR((byte*)"clie1");
    memcpy(txData, msg, strlen(msg));
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
        break;
      }  
    }
    Mirf.getData(rxData);
    Serial.print("Ricevo: ");
    sprintf(msg, "%-30.30s", rxData);
    Serial.println(msg);
    sprintf(msg, "%-10.10s", rxData);
    if (atol(msg) ==timet)
      segnale =0;
    Serial.print("Valori: ");
    int  long  appo =atol(msg);
    sprintf(msg, "rit %10ld tim %10ld sig %d", appo, timet, segnale);
    Serial.println(msg);
  }
}

