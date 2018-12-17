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

//char  nomeRicettore[] ="Condizionatore Sala";
//char  nomeRicettore[] ="Condizioantore Camera Sud";
//char  nomeRicettore[] ="Condizioantore Camera Nord";
//char  nomeRicettore[] ="Stufa Bagno";

// power ON intero grezzo  
unsigned int powerButton[] = {1304,376,1304,376,464,1212,1304,376,1300,376,468,1212,464,1212,468,1212,464,1208,472,1212,460,1212,1308,7084,1308,376,1300,372,468,1212,1304,376,1304,372,464,1212,468,1212,464,1212,468,1212,464,1212,468,1212,1304,7084,1308,372,1304,372,468,1212,1304,372,1308,372,464,1212,468,1212,464,1212,468,1212,464,1212,468,1212,1304,7084,1308,372,1304,372,468,1212,1304,372,1308,372,464,1212,468,1212,464,1212,468,1212,464,1212,468,1216,1300,7084,1308,372,1304,372,468,1216,1300,372,1308,372,464,1212,472,1208,464,1212,468,1212,464,1212,468,1212,1304,7088,1304,372,1304,372,468,1208,1308,376,1304,372,464,1212,468,1212,464,1216,464,1212,464,1212,468,1212,1304,7084,1308,372,1304,372,468,1212,1304,372,1308,372,464,1212,468,1212,464,1212,468,1212,464,1212,472,1208,1304,7084,1308,372,1304,372,468,1212,1304,372,1308,372,464,1212,464,1212,472,1208,464,1212,472,1208,464,1212,1308};
unsigned int highButton[] = {1308,376,1304,368,472,1208,1308,372,1308,368,468,1208,472,1208,1308,372,468,1208,468,1212,468,1208,472,7916,1308,372,1308,372,468,1208,1308,372,1308,368,468,1208,476,1204,1308,372,468,1208,468,1212,468,1212,468,7916,1308,376,1304,372,468,1212,1304,368,1312,372,464,1208,472,1208,1308,372,468,1208,472,1208,468,1212,468,7916,1312,372,1304,372,468,1212,1304,372,1308,368,468,1208,472,1208,1312,368,468,1208,468,1212,468,1208,472,7916,1312,372,1304,372,468,1212,1304,372,1308,372,468,1204,476,1204,1308,372,468,1208,468,1212,468,1208,472,7916,1312,368,1312,368,468,1208,1308,372,1308,372,468,1208,472,1204,1308,372,468,1208,468,1212,468,1212,468,7916,1312,368,1312,368,468,1208,1312,368,1304,376,468,1208,468,1212,1304,376,468,1204,468,1212,468,1208,472,7916,1312,368,1312,368,468,1208,1312,368,1308,372,468,1208,468,1212,1304,376,468,1208,468,1204,476,1204,472};
unsigned int lowButton[] = {1284,396,1308,372,440,1240,1276,400,1304,376,464,1216,464,1212,436,1240,1280,396,440,1240,440,1236,468,7924,1304,376,1304,376,464,1212,1304,376,1304,368,468,1212,464,1212,468,1212,1308,372,464,1212,468,1212,464,7924,1308,372,1304,372,468,1212,1304,372,1304,376,468,1212,464,1208,472,1208,1308,368,468,1212,468,1212,464,7924,1304,372,1308,372,464,1212,1308,372,1308,372,464,1212,468,1212,464,1212,1304,376,464,1216,464,1212,468,7920,1304,376,1304,372,468,1212,1304,376,1300,372,472,1208,464,1212,468,1212,1308,372,464,1212,468,1208,468,7920,1312,368,1308,372,464,1212,1308,372,1308,372,464,1212,468,1212,464,1212,1304,376,464,1212,468,1212,468,7920,1308,372,1304,372,468,1212,1308,368,1304,376,464,1212,468,1212,468,1212,1304,368,472,1208,464,1212,468,7924,1304,376,1304,376,464,1212,1304,376,1304,368,472,1208,464,1212,468,1212,1308,372,464,1212,468,1212,464};


unsigned int signal1 = 10;
unsigned int signal2 = 210;
unsigned int signal3 = 410;
unsigned int signal4 = 610;
unsigned int incremento = 0;

int led = 13;
int ix;


void setup()
{
  Serial.begin(9600);
  irsend.enableIROut(khz);
  delay(1000);
  Serial.println(F("AnalysIR"));
  pinMode(13, OUTPUT);

 // if (!strcmp(nomeRicettore, "Condizionatore Sala"))
  //  incremento =11000;
//  if (!strcmp(nomeRicettore, "Condizioantore Camera Sud"))
//    incremento =21000;
//  if (!strcmp(nomeRicettore, "Condizioantore Camera Sud"))
//    incremento =31000;
//  if (!strcmp(nomeRicettore, "Stufa Bagno"))
    incremento =41000;    
  
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

void loop()
{  
  if (mySwitch.available())
  {  
    int value = mySwitch.getReceivedValue();
    
    if (value == 0)
      Serial.println("Unknown encoding");
    else
    {  
      Serial.print(F("Received "));
      Serial.println( mySwitch.getReceivedValue() );
//      Serial.print(" / ");
//      Serial.println( mySwitch.getReceivedBitlength() );

      if(value == signal1)
        powerOnAndHIgh();
      //if(value == signal2)
        //powerOnAndLow();
      if(value == signal3)
        powerOnOff();
//      if(value == signal4)
//       eseguiIR_4();
    }
    mySwitch.resetAvailable();
  }
}





void powerOnAndHIgh()
{
    digitalWrite(led, HIGH);
    irsend.sendRaw(powerButton, sizeof(powerButton) / sizeof(int), khz);
    delay(500);
    digitalWrite(led, LOW);
    
    delay(2000);
    
    digitalWrite(led, HIGH);
    irsend.sendRaw(highButton, sizeof(highButton) / sizeof(int), khz);
    delay(500);
    digitalWrite(led, LOW);
    
    // per evitare di ricevere più volte la ridondanza dello stesso comando
    delay(5000);
}

/*
void powerOnAndLow()
{
    digitalWrite(led, HIGH);
    irsend.sendRaw(powerButton, sizeof(powerButton) / sizeof(int), khz);
    delay(500);
    digitalWrite(led, LOW);
    
    delay(2000);
    
    digitalWrite(led, HIGH);
    irsend.sendRaw(lowButton, sizeof(lowButton) / sizeof(int), khz);
    delay(500);
    digitalWrite(led, LOW);
    
    // per evitare di ricevere più volte la ridondanza dello stesso comando
    delay(5000);
}
*/

void powerOnOff()
{
    digitalWrite(led, HIGH);
    irsend.sendRaw(powerButton, sizeof(powerButton) / sizeof(int), khz);
    delay(500);
    digitalWrite(led, LOW);
    
    // per evitare di ricevere più volte la ridondanza dello stesso comando
    delay(5000);
}

