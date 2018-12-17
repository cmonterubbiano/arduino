/*
 * Allarme volumetrico pin 7 
 * con cicalino passivi pin 8
 * chiamata web pin 10-11-12-13, con servizio IFTTT 
 */
 
#include <SPI.h>
#include <EtherCard.h>
/*
https://maker.ifttt.com/trigger/allarme/with/key/gIPjQHXG7GUwbxb_Tn7K3OXRySwQaf6oXyh2QnF1Qod
*/
// ethernet interface mac address, must be unique on the LAN
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };

// VALORI ORIGINALI
//char postVariableToURL[250];
//byte Ethernet::buffer[700];

char postVariableToURL[100];

byte Ethernet::buffer[900];
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
char* colorLabel;

int pir1Pin =14;  // Input for HC-S501 il pin 14 -> A0 come digitale terrazzo garibaldi
int pir2Pin =15;  // Input for HC-S501 il pin 15 -> A1 come digitale finestra bagno
int pir3Pin =16;  // Input for HC-S501 il pin 16 -> A2 come digitale terrazzo giardino
int pir4Pin =17;  // Input for HC-S501 il pin 17 -> A3 come digitale corridoio camere
int pir5Pin =18;  // Input for HC-S501 il pin 18 -> A4 come digitale corridoio ingresso

int ledPin =9;  // LED stato generale
int ledHin =5;  // LED stato home
int ledOin =6;  // LED stato acceso

int	int1Pin =8;		// Controllo cavo pir 1 integro 0 interrotto garibaldi
int	int2Pin =7;		// Controllo cavo pir 1 integro 0 interrotto bagno
int	int3Pin =4;		// Controllo cavo pir 1 integro 0 interrotto giardino

int	pirPin;			// Pir incriminato
int	intPin;			// Filo interrotto

int	valled;

int pirValue; // Place to store read PIR Value
int incorso;
long  tempo_disattivo =180000L;  // Tempo in millisecondi di interdizione dopo allarme

static void my_callback (byte status, word off, word len)
{
	Serial.println("<<<");
	Ethernet::buffer[off+300] = 0;
	Serial.println("...");
	Serial.print((const char*) Ethernet::buffer + off);
	Serial.println(" >>>");
	ether.hisport =82;

  	if(ledStatus)
	{
		analogWrite(ledPin, valled);
		if (!strcmp(statusLabel, home))
				colorLabel = "green";
		else	colorLabel = "red";
	}
	else
	{
		digitalWrite(ledPin, LOW);
		colorLabel = "black";
	}
	//delay(tempo_disattivo);
}

void setup()
{
	Serial.begin(9600);
	Serial.println(F("\n[webClient]"));

	if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
		Serial.println(F("Failed to access Ethernet controller"));
    
	ether.staticSetup(myip, gwip, dnsip, mask);
	ether.hisport =82;

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
	pinMode(ledHin, OUTPUT);
	pinMode(ledOin, OUTPUT);
	pinMode(pir1Pin, INPUT);
	pinMode(pir2Pin, INPUT);
	pinMode(pir3Pin, INPUT);
	pinMode(pir4Pin, INPUT);
	pinMode(pir5Pin, INPUT);
	pinMode(int1Pin, INPUT);
	pinMode(int2Pin, INPUT);
	pinMode(int3Pin, INPUT);
 
	digitalWrite(ledPin, LOW);
	digitalWrite(ledHin, LOW);
	digitalWrite(ledOin, LOW);

	delay(10000);
	Serial.println(F("Pronto se SRV e' diverso da zero"));
	ledStatus = false;
	statusLabel = off;
}

int stato;
long	inizio_all;

void loop()
{
  	word len = ether.packetReceive();
	word pos = ether.packetLoop(len);
	
	if (incorso
	&& (millis() < inizio_all || ((millis() -inizio_all) >=tempo_disattivo)))
	{
		Serial.println(millis());
		Serial.println(inizio_all);
		Serial.println("Controllo attivo");
		incorso =0;
	}
	
	if(pos)
	{
		if(strstr((char *)Ethernet::buffer + pos, "GET /?status=ON") != 0)
		{
			Serial.println("Received ON command");
			ledStatus = true;
			statusLabel = on;
			valled =128;
			analogWrite(ledOin, 128);
			digitalWrite(ledHin, LOW);
		}
		
		if(strstr((char *)Ethernet::buffer + pos, "GET /?status=HOME") != 0)
		{
			Serial.println("Received HOME command");
			ledStatus = true;
			statusLabel = home;
			valled =64;
			analogWrite(ledHin, 200);
			digitalWrite(ledOin, LOW);
		}

		if(strstr((char *)Ethernet::buffer + pos, "GET /?status=OFF") != 0)
		{
			Serial.println("Received OFF command");
			ledStatus = false;
			statusLabel = off;
		}
		if(ledStatus)
		{
			analogWrite(ledPin, valled);
			if (!strcmp(statusLabel, home))
					colorLabel = "green";
			else	colorLabel = "red";
		}
		else
		{
			digitalWrite(ledPin, LOW);
			digitalWrite(ledHin, LOW);
			digitalWrite(ledOin, LOW);
			colorLabel = "black";
		}
		
		BufferFiller bfill = ether.tcpOffset();

		bfill.emit_p(PSTR("HTTP/1.0 200 OK\r\n"
		"Content-Type: text/html\r\nPragma: no-cache\r\n\r\n"
		"<html><head><title>Controllo Allarme</title></head>"
		"<body>"

		"<center>"
			"<font size=\"10\">Stato Allarme"
				"<h1>"
				"<font color=$S>"
				"$S"
				"</font>"
				"</h1>"
				"<hr>"
				"<table class=\"mia_tabella\" >"
					"<tr>"
						"<td class=\"colonna_1\" >"
							"<a href=\"/?status=OFF\">"
								"<input type=\"button\" value=\"OFF\" >"
							"</a>"
						"</td>"
						"<td class=\"colonna_2\">"
							"<a href=\"/?status=CECK\">"
								"<input type=\"button\" value=\"CECK\" >"
							"</a>"
						"</td>"
					"</tr>"
					"<tr>"
						"<td>"
							"<a href=\"/?status=ON\">"
								"<input type=\"button\" value=\"ON\" >"
							"</a>"
						"</td>"
						"<td>"
							"<a href=\"/?status=HOME\">"
								"<input type=\"button\" value=\"HOME\" >"
							"</a>"
						"</td>"
					"</tr>"
				"</table>"
			"</font>"
		"</center>"
	"</body>"
	"<style>"
	 ".bottoni_class {"
		"height: 40px; width: 60px;"
	 "}"	  
	 "a input {"
		"font-size: 50px;"
	 "}"
	
	".mia_tabella {"
		"width: 400px;"
		"margin: auto;"
	"}"
	".mia_tabella td {"
		"height: 80px;"
		"min"
	"}"	
	".colonna_1 {"
		"width: 70%;"
	"}"
	"</style>"
	"</html>"
		), colorLabel, statusLabel);

		ether.httpServerReply(bfill.position());
	}
	
	pirPin =intPin =0;
	
	pirValue = digitalRead(pir1Pin);
	if (pirValue)
		pirPin =1;
	if (!pirPin)
	{
		pirValue = digitalRead(pir2Pin);
		if (pirValue)
			pirPin =2;
	}
	if (!pirPin)
	{
		pirValue = digitalRead(pir3Pin);
		if (pirValue)
			pirPin =3;
	}
	if (!pirPin && strcmp(statusLabel, home))
	{
		pirValue = digitalRead(pir4Pin);
		if (pirValue)
			pirPin =4;
	}
	if (!pirPin && strcmp(statusLabel, home))
	{
		pirValue = digitalRead(pir5Pin);
		if (pirValue)
			pirPin =5;
	}
	if (!digitalRead(int1Pin))
		intPin =1;
	if (!intPin && !digitalRead(int2Pin))
		intPin =2;
	if (!intPin && !digitalRead(int3Pin))
		intPin =3;

	if (!incorso
	&& (stato != pirValue || intPin)) 
	{
		if (intPin)
			pirValue =HIGH;
		
		stato =pirValue;
		if (pirValue)
		{
			int i;
			char appo[20];

			Serial.print(F("Acceso - Pir :"));
			switch(pirPin)
			{
				case 0:
						sprintf(appo, "%-10.10s", "");
						break;
				case 1:
						sprintf(appo, "%-10.10s", "Garibaldi");
						break;
				case 2:
						sprintf(appo, "%-10.10s", "Garage");
						break;
				case 3:
						sprintf(appo, "%-10.10s", "Giardino");
						break;
				case 4:
						sprintf(appo, "%-10.10s", "Ingresso");
						break;
				case 5:
						sprintf(appo, "%-10.10s", "Camere");
						break;
			}
			Serial.print(appo);
			Serial.print(" - Filo : ");
			switch(intPin)
			{
				case 0:
						sprintf(appo, "%-10.10s", "");
						break;
				case 1:
						sprintf(appo, "%-10.10s", "Garibaldi");
						break;
				case 2:
						sprintf(appo, "%-10.10s", "Garage");
						break;
				case 3:
						sprintf(appo, "%-10.10s", "Giardino");
						break;
				case 4:
						sprintf(appo, "%-10.10s", "Camere");
						break;
				case 5:
						sprintf(appo, "%-10.10s", "Ingresso");
						break;
			}
			Serial.println(appo);
			for(i=0; i<50; i++)// Wen a frequency sound
			{
				analogWrite(ledPin, 128);
				delay(1);//delay 1ms
				digitalWrite(ledPin, LOW);
				delay(100);// delay 1ms
			}

			if (ledStatus && !incorso)
			{
				inizio_all =millis();
				incorso =1;
				ether.hisport =80;
				sendValueToRaspViaLan("allarme");
			}
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
