// Demo using DHCP and DNS to perform a web client request.
// 2011-06-08 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

#include <EtherCard.h>
#include <stdio.h>
#include <string.h>

#define ENC28J60_CS 53
// ethernet interface mac address, must be unique on the LAN
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };

byte Ethernet::buffer[700];
static uint32_t timer;

//static byte hisip[] = { 216,58,210,196 };
//static byte hisip[] = { 192,168,1,133 };
static byte hisip[] = { 192,168,1,241 };

//const char website[] PROGMEM = "216.58.210.196";
//const char website[] PROGMEM = "192.168.1.133";
const char website[] PROGMEM = "192.168.1.241";
//const char website[] PROGMEM = "www.google.com";
int  padre =0;

// called when the client request is complete
static void my_callback (byte status, word off, word len)
{
  int  ix;
  
  for (ix =off; ix <(off +len) ;ix++)
  {
    if (!strncmp((const char*) Ethernet::buffer + ix, "CIAO", 4))
      break;
  }
  Serial.println(">>>");
  Ethernet::buffer[off+300] = 0;
  Serial.print((const char*) Ethernet::buffer + ix);
  Serial.println("...");
  Serial.print("status ");
  Serial.print(status);
  Serial.print(" len ");
  Serial.println(len);
  Serial.print(" ix ");
  Serial.println(ix);
  Serial.print(" off ");
  Serial.println(off);
  
  if (strstr((char *) Ethernet::buffer + off, "PADRE"))
    padre =1;
  else
    padre =0;
}

void setup ()
{
  Serial.begin(9600);
  Serial.println(F("\n[webClient]"));


  if (!ether.begin(sizeof Ethernet::buffer, mymac, ENC28J60_CS))
    Serial.println("ERROR: ether.begin!");

  if (!ether.dhcpSetup())
    Serial.println(F("DHCP failed"));

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);  
  ether.printIp("DNS: ", ether.dnsip);  

  /*if (!ether.dnsLookup(website))
    Serial.println("DNS failed");*/
    
  ether.copyIp(ether.hisip, hisip);
  
  ether.printIp("SRV: ", ether.hisip);
}

void loop ()
{
  ether.packetLoop(ether.packetReceive());

  if (millis() > timer) 
  {
    timer = millis() + 150;
    Serial.println();
    Serial.print("<<< REQ ");
 
    if(!padre) 
      ether.browseUrl(PSTR("/off"), "", website, my_callback);
    else
      ether.browseUrl(PSTR("/on"), "", website, my_callback);
  }
}
