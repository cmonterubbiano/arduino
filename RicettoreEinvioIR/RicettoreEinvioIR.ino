/*
  Simple example for receiving
  
  http://code.google.com/p/rc-switch/
*/

#include <RCSwitch.h>
#include <IRremote.h>

IRsend irsend;

//You could also try 36,40 & 56 frequencies as a test NORMAL = 38
int khz = 38; //NB Change this default value as neccessary to the correct modulation frequency

RCSwitch mySwitch = RCSwitch();

char nomeRicettore[] ="Condizioantore Sala";
//char nomeRicettore = String("Condizioantore Camera Sud");
//char nomeRicettore = String("Condizioantore Camera Nord");
//char nomeRicettore = String("Stufa Bagno");

// toshiba mio accensione
unsigned int segnaleIR_1[] = {4416, 4312, 544, 1604, 552, 1604, 568, 1584, 544, 1604, 544, 532, 548, 528, 572, 1580, 548, 528, 544, 532, 544, 532, 544, 532, 540, 560, 544, 1580, 572, 1584, 540, 532, 544, 1608, 572, 504, 544, 532, 544, 532, 544, 1608, 568, 508, 544, 532, 540, 1608, 544, 1608, 548, 1604, 568, 1584, 544, 1604, 544, 532, 572, 1580, 548, 1604, 568, 508, 568, 508, 544, 528, 548, 532, 540, 532, 548, 528, 548, 532, 568, 504, 544, 536, 564, 1584, 544, 532, 540, 1612, 544, 1608, 568, 508, 540, 536, 544, 528, 572, 504, 548, 528, 548, 528, 548, 1604, 568, 508, 568, 508, 544, 532, 572, 504, 540, 1608, 572, 1584, 540, 532, 544, 532, 544, 532, 544, 532, 540, 536, 540, 532, 548, 528, 548, 532, 568, 508, 568, 508, 540, 1608, 572, 504, 544, 532, 544, 532, 544, 1608, 568, 508, 544, 7428, 4360, 4340, 544, 1608, 568, 1584, 568, 1584, 540, 1608, 544, 532, 548, 528, 544, 1608, 544, 532, 544, 532, 544, 532, 540, 536, 540, 532, 548, 1608, 544, 1608, 564, 508, 572, 1580, 540, 536, 544, 528, 572, 508, 544, 1608, 540, 536, 540, 532, 572, 1580, 548, 1628, 544, 1584, 568, 1584, 540, 1612, 568, 504, 572, 1584, 564, 1608, 548, 504, 572, 504, 572, 504, 540, 536, 568, 504, 572, 508, 568, 508, 544, 532, 540, 536, 540, 1608, 544, 532, 544, 1608, 544, 1608, 544, 556, 520, 556, 520, 556, 544, 508, 540, 532, 544, 532, 544, 1636, 544, 532, 516, 532, 568, 532, 520, 532, 572, 1580, 540, 1612, 568, 504, 572, 504, 572, 508, 568, 508, 540, 536, 540, 532, 544, 532, 540, 564, 512, 536, 544, 528, 572, 1584, 544, 532, 564, 536, 544, 508, 540, 1608, 544, 532, 544};
//toshiba spegnimento
unsigned int segnaleIR_4[] = {4388, 4340, 544, 1604, 572, 1580, 572, 1580, 544, 1608, 568, 508, 544, 532, 544, 1604, 572, 508, 544, 532, 540, 532, 568, 508, 544, 532, 572, 1580, 548, 1604, 544, 532, 544, 1604, 572, 504, 544, 532, 572, 504, 572, 1580, 568, 508, 568, 508, 544, 1604, 576, 1576, 572, 1584, 544, 1604, 544, 1608, 544, 532, 572, 1576, 548, 1608, 544, 532, 564, 508, 572, 504, 544, 532, 572, 504, 548, 524, 548, 532, 544, 532, 544, 532, 568, 1580, 544, 532, 572, 1584, 540, 1608, 544, 532, 572, 504, 572, 504, 568, 504, 572, 508, 568, 508, 568, 1580, 572, 504, 572, 504, 572, 504, 572, 1604, 544, 1584, 572, 1608, 512, 532, 548, 532, 544, 532, 544, 532, 540, 532, 544, 532, 572, 504, 544, 532, 548, 528, 544, 532, 544, 1608, 568, 508, 540, 536, 568, 1580, 572, 1580, 548, 532, 540, 7428, 4364, 4340, 540, 1636, 544, 1580, 548, 1600, 572, 1584, 544, 532, 540, 536, 540, 1608, 572, 504, 572, 532, 516, 536, 564, 508, 568, 508, 572, 1580, 544, 1604, 572, 504, 548, 1608, 540, 536, 540, 556, 544, 508, 544, 1608, 540, 532, 548, 528, 548, 1632, 520, 1608, 568, 1580, 544, 1608, 544, 1608, 568, 508, 544, 1608, 568, 1580, 548, 528, 548, 532, 564, 512, 540, 556, 520, 532, 544, 532, 544, 528, 572, 504, 572, 508, 544, 1608, 540, 532, 544, 1608, 544, 1608, 572, 508, 540, 532, 568, 508, 544, 532, 596, 480, 572, 504, 540, 1608, 572, 504, 572, 508, 544, 532, 564, 1584, 544, 1608, 540, 1612, 544, 532, 544, 532, 544, 532, 544, 556, 540, 512, 540, 532, 572, 504, 544, 532, 544, 536, 564, 508, 568, 1584, 544, 532, 544, 528, 572, 1580, 548, 1604, 544, 532, 572};

unsigned int segnaleIR[] = {4388, 4340, 544, 1604, 572, 1580, 572, 1580, 544, 1608, 568, 508, 544, 532, 544, 1604, 572, 508, 544, 532, 540, 532, 568, 508, 544, 532, 572, 1580, 548, 1604, 544, 532, 544, 1604, 572, 504, 544, 532, 572, 504, 572, 1580, 568, 508, 568, 508, 544, 1604, 576, 1576, 572, 1584, 544, 1604, 544, 1608, 544, 532, 572, 1576, 548, 1608, 544, 532, 564, 508, 572, 504, 544, 532, 572, 504, 548, 524, 548, 532, 544, 532, 544, 532, 568, 1580, 544, 532, 572, 1584, 540, 1608, 544, 532, 572, 504, 572, 504, 568, 504, 572, 508, 568, 508, 568, 1580, 572, 504, 572, 504, 572, 504, 572, 1604, 544, 1584, 572, 1608, 512, 532, 548, 532, 544, 532, 544, 532, 540, 532, 544, 532, 572, 504, 544, 532, 548, 528, 544, 532, 544, 1608, 568, 508, 540, 536, 568, 1580, 572, 1580, 548, 532, 540, 7428, 4364, 4340, 540, 1636, 544, 1580, 548, 1600, 572, 1584, 544, 532, 540, 536, 540, 1608, 572, 504, 572, 532, 516, 536, 564, 508, 568, 508, 572, 1580, 544, 1604, 572, 504, 548, 1608, 540, 536, 540, 556, 544, 508, 544, 1608, 540, 532, 548, 528, 548, 1632, 520, 1608, 568, 1580, 544, 1608, 544, 1608, 568, 508, 544, 1608, 568, 1580, 548, 528, 548, 532, 564, 512, 540, 556, 520, 532, 544, 532, 544, 528, 572, 504, 572, 508, 544, 1608, 540, 532, 544, 1608, 544, 1608, 572, 508, 540, 532, 568, 508, 544, 532, 596, 480, 572, 504, 540, 1608, 572, 504, 572, 508, 544, 532, 564, 1584, 544, 1608, 540, 1612, 544, 532, 544, 532, 544, 532, 544, 556, 540, 512, 540, 532, 572, 504, 544, 532, 544, 536, 564, 508, 568, 1584, 544, 532, 544, 528, 572, 1580, 548, 1604, 544, 532, 572};
unsigned  int  *pint;
unsigned  int  *pun1;
unsigned  int  *pun4;

unsigned int signal1 = 10;
unsigned int signal2 = 210;
unsigned int signal3 = 410;
unsigned int signal4 = 610;
unsigned int incremento = 0;

void setup() {
  Serial.begin(9600);
  irsend.enableIROut(khz);
  delay(1000);
  Serial.println(F("AnalysIR"));
  pinMode(13, OUTPUT);

  
  if( !strcmp(nomeRicettore, "Condizioantore Sala")){incremento = 11000; }
  if( nomeRicettore == "Condizioantore Camera Sud" ){incremento = 21000; }
  if( nomeRicettore == "Condizioantore Camera Nord"){incremento = 31000; }
  if( nomeRicettore == "Stufa Bagno" ){incremento = 41000; }
  
  signal1 = signal1 + incremento;
  signal2 = signal2 + incremento;
  signal3 = signal3 + incremento;
  signal4 = signal4 + incremento;
  
  mySwitch.enableReceive(0);  // Receiver on inerrupt 0 => that is pin #2

      Serial.println(F("singal1: "));
      Serial.println(signal1);
      Serial.println(F("incremento: "));
      Serial.println(incremento);
      
}

void loop() {
  
  if (mySwitch.available()) {
    
    int value = mySwitch.getReceivedValue();
    
    if (value == 0) {
      Serial.print("Unknown encoding");
    } else {
      
      Serial.println(F("Received "));
      Serial.print( mySwitch.getReceivedValue() );
      Serial.print(" / ");
      //Serial.print( mySwitch.getReceivedBitlength() );

      //signal1 = 12844044; //per Debug
  //    if(value == signal1){ eseguiIR_1(); }
      if(value == signal1)
        eseguiIR(1);
//      if(value == signal2){ eseguiIR( segnaleIR_2 ); }
//      if(value == signal3){ eseguiIR( segnaleIR_3 ); }
//      if(value == signal4){ eseguiIR_4(); }
     if(value == signal4)
       eseguiIR(4);
      
    }

    mySwitch.resetAvailable();
    
  }
  
}
void eseguiIR(int  ix)
{
  int  iy =sizeof(segnaleIR)/sizeof(int);
  char  mess[80];
  unsigned  int  appo;
  sprintf(mess, "lunghezza %3d ix %d", iy, ix);
  Serial.println(mess);
  if (ix ==1)
  {
    int  iz;
   
    pint =&segnaleIR_1[0];

    irsend.sendRaw(pint, sizeof(segnaleIR)/sizeof(int), khz); //AnalysIR Batch Export (IRremote) - RAW

   //  segnaleIR[iz] =appo;
      sprintf(mess, "punt %4d iy %03d", pint[0], ix);
      Serial.println(mess);
      delay(100);
   
  }
   if (ix ==4)
  {
    int  iz;
   
    pint =&segnaleIR_4[0];

    irsend.sendRaw(pint, sizeof(segnaleIR)/sizeof(int), khz); //AnalysIR Batch Export (IRremote) - RAW

   //  segnaleIR[iz] =appo;
      sprintf(mess, "punt %4d iy %03d", pint[0], ix);
      Serial.println(mess);
      delay(100);
   
  }
//   if (ix ==4)
//     for (; iy >=0; segnaleIR[iy] =segnaleIR_4[iy--]);
//  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
//  irsend.sendRaw(segnaleIR, sizeof(segnaleIR)/sizeof(int), khz); //AnalysIR Batch Export (IRremote) - RAW
//   // AnalysIR IR Protocol: MITSUBISHI288AC, Key:  
//  delay(500);              // wait for a 1/2 second
//  digitalWrite(13, LOW);  // turn the LED off by making the voltage LOW     
//  delay(1000);
}

void  assegna(unsigned  int  val, int  punt)
{
  char  mess[80];
  
  segnaleIR[punt] =val;
        sprintf(mess, "appo %4d iy %03d", val, punt);
      Serial.println(mess);
      delay(100);
}

void eseguiIR_1() {
  Serial.println(F("sending IR "));
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  irsend.sendRaw(segnaleIR_1, sizeof(segnaleIR_1)/sizeof(int), khz); //AnalysIR Batch Export (IRremote) - RAW
   // AnalysIR IR Protocol: MITSUBISHI288AC, Key:  
  delay(500);              // wait for a 1/2 second
  digitalWrite(13, LOW);  // turn the LED off by making the voltage LOW     
  delay(1000);
}


void eseguiIR_4() {
  Serial.println(F("sending IR "));
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  irsend.sendRaw(segnaleIR_4, sizeof(segnaleIR_4)/sizeof(int), khz); //AnalysIR Batch Export (IRremote) - RAW
   // AnalysIR IR Protocol: MITSUBISHI288AC, Key:  
  delay(500);              // wait for a 1/2 second
  digitalWrite(13, LOW);  // turn the LED off by making the voltage LOW     
  delay(1000);
}




