/*
 * Allarme volumetrico pin 7 
 * con cicalino passivi pin 8
 * chiamata web pin 10-11-12-13, con servizio IFTTT 
 */
 
#include <SPI.h>
#include <EtherCard.h>

// ethernet interface mac address, must be unique on the LAN
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };

//char postVariableToURL[250];
char postVariableToURL[150];

byte Ethernet::buffer[700];
static uint32_t timer;

static byte myip[] = { 192,168,1,208 };
static byte gwip[] = { 192,168,1,1 };
static byte dnsip[] = { 192,168,1,1 };
static byte mask[] = { 255,255,255,0 };
//static byte hisip[] = { 208,86,224,90 };
static byte hisip[] = { 192,168,1,208 };

const char website[] PROGMEM = "maker.ifttt.com";
//const char website[] PROGMEM = "www.google.com";
//const char website[] PROGMEM = "192.168.1.203";
// called when the client request is complete}
boolean ledStatus;

char* on = "ON";
char* off = "OFF";
char* home = "HOME";

char* statusLabel;
char* buttonLabel;

int ledPin =9;  // LED on Pin 6 of Arduino
int pir1Pin =14;  // Input for HC-S501 il pin 14 corrisponde al A0 come digitale
int pir2Pin =15;  // Input for HC-S501 il pin 14 corrisponde al A0 come digitale
int buzzer =8;  // Set the control the buzzer digital IO pin

int  valled;

int pirValue; // Place to store read PIR Value
int incorso;
long  tempo_disattivo =30000L;  // Tempo in millisecondi di interdizione dopo allarme


void setup()
{
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);
  pinMode(pir1Pin, INPUT);

 
  digitalWrite(ledPin, LOW);
  
  Serial.println(F("Pronto"));
  ledStatus = false;
  statusLabel = off;
}

int stato;

void loop()
{
  
  pirValue = digitalRead(pir1Pin);
  //digitalWrite(ledPin, pirValue);
  if (stato != pirValue && !incorso)
  {
    stato =pirValue;
    if (pirValue)
    {
      int i;
 
      Serial.println(F("Acceso"));
      for(i=0; i<50; i++)// Wen a frequency sound
      {
        digitalWrite(buzzer,HIGH);// Sound
        analogWrite(ledPin, 255);
        delay(1);//delay 1ms
        digitalWrite(buzzer,LOW);//Not sound
        digitalWrite(ledPin, LOW);
        delay(100);// delay 1ms
      }
 
    }
    else  Serial.println(F("Spento"));
  }
}

