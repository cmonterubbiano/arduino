/*
 * Allarme volumetrico pin 7 
 * con cicalino passivi pin 8
 * chiamata web pin 10-11-12-13, con servizio IFTTT 
 */
 
 
#include <SPI.h>
#include <EtherCard.h>

// ethernet interface mac address, must be unique on the LAN
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };

char postVariableToURL[250];

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



int ledPin =9;  // LED on Pin 6 of Arduino
int pirPin =14;  // Input for HC-S501
int buzzer =8;  // Set the control the buzzer digital IO pin

int pirValue; // Place to store read PIR Value
int incorso;
long  tempo_disattivo =30000L;  // Tempo in millisecondi di interdizione dopo allarme

static void my_callback (byte status, word off, word len)
{
  Serial.println(">>>");
  Ethernet::buffer[off+300] = 0;
  Serial.print((const char*) Ethernet::buffer + off);
  Serial.println("...");
  delay(tempo_disattivo);
  Serial.println(">>>");
  incorso =0;
}

void setup()
{
  Serial.begin(9600);
  Serial.println(F("\n[webClient]"));

  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
    Serial.println(F("Failed to access Ethernet controller"));
    
  ether.staticSetup(myip, gwip, dnsip, mask);

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);  
  ether.printIp("DNS: ", ether.dnsip);  

// via nome dominio
  if (!ether.dnsLookup(website))
    Serial.println("DNS failed");
    
// via indirizzo IP (v4) statico    
  //char websiteIP[] = "192.168.1.203";  ether.parseIp(ether.hisip, websiteIP);
    
  ether.printIp("SRV: ", ether.hisip);
  delay(2000);

  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);
  pinMode(buzzer, OUTPUT);
 
  digitalWrite(ledPin, LOW);
  
  Serial.println(F("Pronto se SRV e' diverso da zero"));
  
}

int stato;

void loop()
{
  ether.packetLoop(ether.packetReceive());
  
  pirValue = digitalRead(pirPin);
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
        digitalWrite(ledPin, HIGH);
        delay(1);//delay 1ms
        digitalWrite(buzzer,LOW);//Not sound
        digitalWrite(ledPin, LOW);
        delay(100);// delay 1ms
      }
      //0. righe commentate per no far partire la chiamata IFTTT
      incorso =1;
      sendValueToRaspViaLan("allarme");
    }
    else  Serial.println(F("Spento"));
  }
}

void sendValueToRaspViaLan(char* value)
{
  ether.packetLoop(ether.packetReceive());
  sprintf(postVariableToURL, "trigger\/%s\/with\/key\/gIPjQHXG7GUwbxb_Tn7K3OXRySwQaf6oXyh2QnF1Qod", value);
  Serial.println(postVariableToURL);                             
  ether.browseUrl(PSTR("/"), postVariableToURL, website, my_callback);
}
