// Demo using DHCP and DNS to perform a web client request.
// 2011-06-08 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php


//http s://maker.ifttt.com/trigger/allarme/with/key/gIPjQHXG7GUwbxb_Tn7K3OXRySwQaf6oXyh2QnF1Qod

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
static byte hisip[] = { 192,168,1,203 };

const char website[] PROGMEM = "maker.ifttt.com";
//const char website[] PROGMEM = "www.google.com";
//const char website[] PROGMEM = "192.168.1.203";

// called when the client request is complete
static void my_callback (byte status, word off, word len) {
  Serial.println(">>>");
  Ethernet::buffer[off+300] = 0;
  Serial.print((const char*) Ethernet::buffer + off);
  Serial.println("...");
}

void setup () {
  Serial.begin(9600);
  Serial.println(F("\n[webClient]"));

  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
    Serial.println(F("Failed to access Ethernet controller"));
  //if (!ether.dhcpSetup())
    //Serial.println(F("DHCP failed"));
  ether.staticSetup(myip, gwip, dnsip, mask);

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);  
  ether.printIp("DNS: ", ether.dnsip);  

// via nome dominio
  if (!ether.dnsLookup(website))   Serial.println("DNS failed");
    
// via indirizzo IP (v4) statico    
  //char websiteIP[] = "192.168.1.203";  ether.parseIp(ether.hisip, websiteIP);
    
  ether.printIp("SRV: ", ether.hisip);

  delay(2000);
  sendValueToRaspViaLan("allarme");
}

void loop () {
  ether.packetLoop(ether.packetReceive());

/*
  if (millis() > timer) {
    timer = millis() + 5000;
    Serial.println();
    Serial.print("<<< REQ ");
    //ether.browseUrl(PSTR("/"), "", website, my_callback);

  //  ether.browseUrl(PSTR("/receiveArduino.php"), "", website, my_callback);
    sendValueToRaspViaLan(789456);
  }
*/

}


void sendValueToRaspViaLan(char* value) {
  ether.packetLoop(ether.packetReceive());
  Serial.print("<<< REQ ");
  sprintf(postVariableToURL, "?message=%s", value);
  //ether.browseUrl(PSTR("/receiveArduino.php"), postVariableToURL, website, my_callback);
  //ether.browseUrl(PSTR(":443/"), "", website, my_callback);
  ether.browseUrl(PSTR("/"), "trigger/allarme/with/key/gIPjQHXG7GUwbxb_Tn7K3OXRySwQaf6oXyh2QnF1Qod", website, my_callback);
}

