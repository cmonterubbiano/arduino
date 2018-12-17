#include <EtherCard.h>
 
static byte mac[] = { 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0x01 };
static byte ip[] = { 192, 168, 1, 241 };
byte Ethernet::buffer[700];
 
#define ENC28J60_CS 53
#define LED 13
  
const char accendiLED[] PROGMEM = "HTTP/1.0 200 OK\r\n"
"Content-Type: text/html\r\n"
"Pragma: no-cache\r\n\r\n"
"<html><head><title>ARDUINO - ENC28J60</title></head>"
"<body><center>"
"<h1> - Arduino in rete - </h1>"
"<h2> - Test led - </h2>"
"<a href='/on'><input type='button' value=' - ACCENDI LED - '/></a>"
"</body>"
"</center></html>";
  
const char spegniLED[] PROGMEM = "HTTP/1.0 200 OK\r\n"
"Content-Type: text/html\r\n"
"Pragma: no-cache\r\n\r\n"
"<html><head><title>ARDUINO - ENC28J60</title></head>"
"<body><center>"
"<h1> - Arduino in rete - </h1>"
"<h2> - Test led - </h2>"
"<a href='/off'><input type='button' value=' - SPEGNI LED - '/></a>"
"</center></body>"
"</html>";
 
void setup ()
{
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  Serial.println("ARDUINO IN RETE");
  Serial.println(" - TEST LED - ");
  Serial.println(" ");

  if ( !ether.begin(sizeof Ethernet::buffer, mac, ENC28J60_CS) )
   Serial.println("ERROR: ether.begin!");

  if (!ether.dhcpSetup())
   if (!ether.staticSetup(ip))
     Serial.println("ERROR: Set IP failed!");

  ether.printIp("Ip locale: ", ether.myip);
  ether.printIp("Netmask: ", ether.netmask);
  ether.printIp("Gateway: ", ether.gwip);
  ether.printIp("Dns: ", ether.dnsip);
}
 
void loop()
{
  word pos = ether.packetLoop( ether.packetReceive() );
 
  BufferFiller bfill = ether.tcpOffset();

  if (pos)
  {
    Serial.println(" ");
    Serial.println("<hr class='bbcode_rule'> inizio");
    Serial.println( (char *) Ethernet::buffer + pos );
    Serial.println("<hr class='bbcode_rule'> fine");
    Serial.println("");
    Serial.println(" ");

    if (strstr((char *) Ethernet::buffer + pos, "GET /on"))
    {
      digitalWrite(LED, HIGH);
      Serial.println("Led acceso!");
      bfill.emit_p(PSTR("$F"), spegniLED);
      ether.httpServerReply( bfill.position() );
    }
    else if (strstr( (char *) Ethernet::buffer + pos, "GET /off"))
    {
      digitalWrite(LED, LOW);
      Serial.println("Led spento!");
      bfill.emit_p(PSTR("$F"), accendiLED);
      ether.httpServerReply(bfill.position());
    }
    else if ( strstr( (char *) Ethernet::buffer + pos, "GET /led") )
    {
      if ((int) digitalRead(LED))
      {
        bfill.emit_p(PSTR("$F"), spegniLED);
        ether.httpServerReply(bfill.position());
        Serial.println("Spegni LED!");
      }
      else
      {
        bfill.emit_p(PSTR("$F"), accendiLED);
        ether.httpServerReply( bfill.position() );
        Serial.println("Accendi LED!");
      }
    }
  }
}
