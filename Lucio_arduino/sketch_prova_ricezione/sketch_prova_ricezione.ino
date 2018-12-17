/*
  Simple example for receiving
  
  http://code.google.com/p/rc-switch/
*/

//#include <IRLib.h>
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
unsigned int segnaleIR_1[] = {3400,1700,432,1298,432,1298,432,432,432,432,432,432,432,1298,432,432,432,432,432,1298,432,1298,432,432,432,1298,432,432,432,432,432,1298,432,1298,432,432,432,1298,432,1298,432,432,432,432,432,1298,432,432,432,432,432,1298,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,1298,432,432,432,432,432,432,432,432,432,432,432,1298,432,432,432,432,432,432,432,432,432,1298,432,1298,432,1298,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,1298,432,1298,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,1298,432,432,432,1298,432,1298,432,432,432,1298,432,1298,432,432,432,1298,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,1298,432,1298,432,1298,432,1298,432,432,432,432,432,432,432,432,432,16872,3400,1700,432,1298,432,1298,432,432,432,432,432,432,432,1298,432,432,432,432,432,1298,432,1298,432,432,432,1298,432,432,432,432,432,1298,432,1298,432,432,432,1298,432,1298,432,432,432,432,432,1298,432,432,432,432,432,1298,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,1298,432,432,432,432,432,432,432,432,432,432,432,1298,432,432,432,432,432,432,432,432,432,1298,432,1298,432,1298,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,1298,432,1298,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,1298,432,432,432,1298,432,1298,432,432,432,1298,432,1298,432,432,432,1298,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,1298,432,1298,432,1298,432,1298,432,432,432,432,432,432,432,432,432}; //AnalysIR Batch Export - RAW

//toshiba spegnimento
//unsigned int segnaleIR_4[] = {3472,1656,464,1248,480,1236,464,400,464,396,464,396,484,1228,452,408,456,404,480,1232,468,1244,492,372,456,1256,468,396,468,396,480,1232,468,1244,484,380,448,1260,456,1256,464,400,480,380,488,1232,464,388,480,384,484,1228,488,380,480,380,484,380,488,376,480,388,480,380,456,408,468,392,468,388,468,400,480,384,464,392,464,396,452,420,464,396,460,400,464,396,464,400,460,400,480,380,460,404,488,376,468,396,468,396,484,380,472,388,484,1228,492,376,456,408,480,384,480,380,464,400,464,1248,468,1244,464,400,464,396,484,384,464,392,480,380,484,380,408,448,456,408,488,376,484,1232,460,1256,484,384,464,400,464,396,484,384,464,392,484,376,468,396,464,396,464,1244,456,408,464,1248,464,1248,464,396,480,1232,456,408,452,1260,484,384,452,404,484,376,468,396,456,404,464,396,464,400,464,392,456,404,456,408,464,392,488,376,480,384,484,384,464,396,464,392,464,400,464,388,456,408,464,400,464,388,488,380,464,396,464,392,484,384,464,404,464,388,488,380,464,396,456,404,468,392,464,396,468,396,464,392,408,452,460,404,464,396,484,380,464,396,464,392,456,412,456,408,464,400,452,408,464,400,408,452,460,396,408,456,464,396,484,380,464,396,480,1232,456,1256,468,1248,480,1232,456,1256,484,380,468,1244,452,16904,3576,1684,464,1260,460,1252,464,396,464,396,464,400,464,1244,464,392,456,408,408,1304,464,1252,480,376,456,1260,464,392,456,420,408,1304,480,1236,464,396,464,1248,464,1248,460,396,456,404,456,1256,408,464,464,396,464,1248,464,392,484,380,480,384,464,392,452,416,452,408,484,388,464,392,484,380,480,384,480,388,452,404,408,452,456,412,452,408,464,396,464,396,464,396,464,400,464,396,464,392,408,456,464,400,408,448,484,384,464,396,480,1232,484,384,452,404,488,376,456,408,464,408,464,1248,464,1248,464,396,464,396,480,380,464,396,480,384,408,448,492,384,488,380,408,452,460,1248,484,1228,456,408,464,396,464,392,484,384,464,396,452,404,464,400,464,392,464,1252,464,396,408,1308,480,1236,464,400,408,1304,480,384,460,1252,464,392,488,380,464,392,492,380,488,376,452,408,484,380,460,404,464,396,480,384,464,392,460,400,464,396,464,400,452,404,408,452,456,412,452,408,464,396,464,396,464,396,464,400,464,396,464,392,408,456,464,400,408,448,460,408,464,396,480,380,464,396,464,396,464,396,464,396,464,396,408,448,412,452,464,396,464,400,484,380,464,400,452,408,408,452,404,456,408,456,484,376,456,408,480,384,464,392,464,400,408,456,408,1304,464,1252,464,1244,416,1300,460,1256,488,384,464,1248,464};//AnalysIR Batch Export RAW


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
      if(value == signal1){ eseguiIR_1(); }
//      if(value == signal2){ eseguiIR( segnaleIR_2 ); }
//      if(value == signal3){ eseguiIR( segnaleIR_3 ); }
      if(value == signal4){ eseguiIR_4(); }
      
    }

    mySwitch.resetAvailable();
    
  }
  
}


void eseguiIR_1() {
  Serial.println(F("sending IR "));
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  //irsend.sendRaw(segnaleIR_1, sizeof(segnaleIR_1)/sizeof(int), khz); //AnalysIR Batch Export (IRremote) - RAW
   // AnalysIR IR Protocol: MITSUBISHI288AC, Key:  
  delay(500);              // wait for a 1/2 second
  digitalWrite(13, LOW);  // turn the LED off by making the voltage LOW     
  delay(1000);
}




void eseguiIR_4() {
  Serial.println(F("sending IR "));
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  //irsend.sendRaw(segnaleIR_4, sizeof(segnaleIR_4)/sizeof(int), khz); //AnalysIR Batch Export (IRremote) - RAW
   // AnalysIR IR Protocol: MITSUBISHI288AC, Key:  
  delay(2000);              // wait for a 1/2 second
  digitalWrite(13, LOW);  // turn the LED off by making the voltage LOW     
  delay(1000);
}





