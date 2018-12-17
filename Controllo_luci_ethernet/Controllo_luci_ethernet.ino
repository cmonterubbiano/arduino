/*
	Controllo Luci 1.0

	Sketch per il controllo remoto di una lampada da Arduino.

	Pietro Lodi Rizzini
	Dicembre 2015
*/
#include <EtherCard.h>

static byte mymac[] = {0xDA,0xDB,0xDC,0x01,0x02,0x03}; //definizione dei parametri base per la connessione alla rete
static byte myip[] = {192,168,1,208};
static byte gwip[] = {192,168,1,1};

byte Ethernet::buffer[700];

const int ledPin = 6;
boolean ledStatus;

char* on = "ON";
char* off = "OFF";
char* statusLabel;
char* buttonLabel;

void setup ()
{
	Serial.begin(9600);
	Serial.println("Controllo Luci");
	if (ether.begin(sizeof Ethernet::buffer, mymac) == 0)
	Serial.println( "Failed to access Ethernet controller"); //avviso via seriale di eventuali errori (solo per debug)
	ether.staticSetup(myip, gwip); //configurazione IP statico (no DHCP) e IP del gateway
	ether.hisport = 80; // porta default HTTP

	Serial.println();

	pinMode(ledPin, OUTPUT);
	digitalWrite(ledPin, LOW);
	ledStatus = false;
}

void loop()
{

	word len = ether.packetReceive();
	word pos = ether.packetLoop(len);

	if(pos)
	{
		if(strstr((char *)Ethernet::buffer + pos, "GET /?status=ON") != 0)
		{
			Serial.println("Received ON command");
			ledStatus = true;
		}

		if(strstr((char *)Ethernet::buffer + pos, "GET /?status=OFF") != 0)
		{
			Serial.println("Received OFF command");
			ledStatus = false;
		}
		if(ledStatus)
		{
			digitalWrite(ledPin, HIGH);
			statusLabel = on;
			buttonLabel = off;
		}
		else
		{
			digitalWrite(ledPin, LOW);
			statusLabel = off;
			buttonLabel = on;
		}

		BufferFiller bfill = ether.tcpOffset();
		bfill.emit_p(PSTR("HTTP/1.0 200 OK\r\n"
		"Content-Type: text/html\r\nPragma: no-cache\r\n\r\n"
		"<html><head><title>Controllo Luci</title></head>"
		"<body>Stato Luci: $S "
		"<a href=\"/?status=$S\"><input type=\"button\" value=\"$S\"></a>"
		"</body></html>"
		), statusLabel, buttonLabel, buttonLabel);
		ether.httpServerReply(bfill.position());
	}
}
