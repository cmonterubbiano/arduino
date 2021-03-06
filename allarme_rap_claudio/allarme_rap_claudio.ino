/*
 * Allarme volumetrico pin 7 
 È stato suggerito che l'equazione descritta è valida solo se la temperatura è pari 
 o superiore a 27 ° C (80 ° F) e l'umidità relativa è pari o superiore al 40%. 
 */
 
#include <SPI.h>
#include "DHT.h"

#define DHT1PIN 2  // inserire il numero del pin a cui collegato sensore => in questo caso sostituisci il 2
#define DHT2PIN 3  //  come sopra ma sostituire il 3
#define DHT3PIN 10  // inserire il numero del pin a cui collegato sensore => in questo caso sostituisci il 2
#define DHT4PIN 11  //  come sopra ma sostituire il 3

#define TEMPOALLARME 10 // Tempo minimo durata allarme o intervallo tra due impulsi consecutivi  

// Uncomment whatever type you're using!
// #define DHT1TYPE DHT11 // DHT 11
#define DHT2TYPE DHT22 // DHT 22 (AM2302)  /// toglila te usi il dht11
//#define DHTTYPE DHT21 // DHT 21 (AM2301) /// toglila te usi il dht11

// *******************   suggerimenti su come collegare il sensore  ****************
// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND (DHT11) 
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

DHT dht1(DHT1PIN, DHT2TYPE);
DHT dht2(DHT2PIN, DHT2TYPE);
DHT dht3(DHT3PIN, DHT2TYPE);
DHT dht4(DHT4PIN, DHT2TYPE);


boolean ledStatus;

char* on = "ON";
char* off = "OFF";
char* home = "HOME";
char* aiuto = "AIUTO";
char* test = "TEST";
char* test_h = "TEST_HOME";

char* statusLabel;
char* colorLabel;

// int pir1Pin =14;  // Input for HC-S501 il pin 14 -> A0 come digitale terrazzo garibaldi
// int pir2Pin =15;  // Input for HC-S501 il pin 15 -> A1 come digitale finestra bagno
// int pir3Pin =16;  // Input for HC-S501 il pin 16 -> A2 come digitale terrazzo giardino
// int pir4Pin =17;  // Input for HC-S501 il pin 17 -> A3 come digitale corridoio camere
// int pir5Pin =18;  // Input for HC-S501 il pin 18 -> A4 come digitale corridoio ingresso
byte  pin_pir	[] ={  14, 15, 16, 17, 18};		//status sensori 0 chiuso 1 aperto

// int int1Pin =8;   // Controllo cavo pir 1 integro 0 interrotto garibaldi
// int int2Pin =7;   // Controllo cavo pir 1 integro 0 interrotto bagno
// int int3Pin =4;   // Controllo cavo pir 1 integro 0 interrotto giardino
byte  pin_int	[] ={ 7, 4};		//status sensori 1 chiuso 0 aperto

int ledPin =9;  // LED stato generale
int ledHin =5;  // LED stato home
int ledOin =6;  // LED stato acceso

int pirPin;     // Pir incriminato
int intPin;     // Filo interrotto
int	pirini;		// Pir iniziale
int	pirfine;	// Pir finale

int valled;
int incorso;
long  tempo_disattivo =180000L;  // Tempo in millisecondi di interdizione dopo allarme

byte	a_status	=0;									//status allarme    0 spento 1 acceso 2 in allarme
int		p_status	[] ={  0, 0, 0, 0, 0};		//status sensori 0 chiuso 1 aperto
int		p_stapre	[] ={  -1, -1, -1, -1, -1};		//status sensori 0 chiuso 1 aperto

int  i_status	[] ={  0, 0, 0};		//status sensori 0 chiuso 1 aperto
int  i_stapre	[] ={  -1, -1, -1};		//status sensori 0 chiuso 1 aperto

String	inputString = "";         // a string to hold incoming data
boolean	stringComplete = false;  // whether the string is complete

int 				pir_inizio;     // Pir incriminato
long unsigned int	inizio_pir =0L; 
long unsigned int	fine_pir =0L; 
long unsigned int	tempoallarme =(TEMPOALLARME * 1000); 

char* p_arduino = "TEST_HOME";

void setup()
{
	int	pir;
	
	Serial.begin(9600);
	Serial.println("Allarme");
  
	dht1.begin();
	dht2.begin();
	dht3.begin();
	dht4.begin();

	pinMode(ledPin, OUTPUT);
	pinMode(ledHin, OUTPUT);
	pinMode(ledOin, OUTPUT);
  
	for (pir =0; pir <5; pir++)
		pinMode(pin_pir[pir], INPUT);
	  
    for (pir =0; pir <2; pir++)
	  pinMode(pin_int[pir], INPUT);
 
	digitalWrite(ledPin, LOW);
	digitalWrite(ledHin, LOW);
	digitalWrite(ledOin, LOW);
	delay(10000);
	
	ledStatus = false;
	statusLabel = off;
	stampa_arduino();
	Serial.println("|allarme|status|SPENTO");
	stampa_arduino();
	Serial.println("_allarme|SPENTO|Accensione Arduino");
	pirini =0;
	pirfine =5;
}

int stato;
long  inizio_all;
char	*data_agg;
char	*ora_agg;

void loop()
{
	int	pir;
	char	appo[70];
	
	if (incorso
	&& (millis() < inizio_all || ((millis() -inizio_all) >=tempo_disattivo)))
	{
		Serial.println("Controllo attivo");
		incorso =0;			
		stampa_arduino();
		Serial.print("|allarme|");
		Serial.print("status|");
		if (!strcmp(statusLabel, on))
			Serial.println("ACCESO");
		else	if (!strcmp(statusLabel, home))
			Serial.println("HOME");
		else	if (!strcmp(statusLabel, test))
			Serial.println("TEST");
		else
			Serial.println("BOH");
		
		delay(200);// delay 1ms
		stampa_arduino();
		Serial.println("_allarme|INTERROTTO|.");
	}
	if (incorso)
	{
		for(int i=0; i<10; i++)// Wen a frequency sound
		{
			analogWrite(ledOin, 128);
			analogWrite(ledHin, 128);
			delay(4);//delay 1ms
			digitalWrite(ledOin, LOW);
			digitalWrite(ledHin, LOW);
			delay(100);// delay 1ms
		}
	}
  	serialEvent(); //call the function
	// print the string when a newline arrives:
	
	if (stringComplete)
	{
		int ix;
		int iy;
		int	iz;
		int	comando =-1;
		int	evento =-1;
		// Serial.println(inputString);
		for(ix =iy =iz =0; ix <inputString.length(); ix++, iz++)
		{
			//	raspberry|allarme|APRI
			if (inputString[ix] =='|')
			{
				iy++;
				appo[iz] ='\0';
				if (iy == 1)
				{
					if (strcmp(appo, "raspberry"))
						break;
				}
				if (iy == 2)
				{
					if (!strcmp(appo, "temperatura"))
					{
						evento =1;
					}
					if (!strcmp(appo, "allarme"))
					{
						evento =0;
					}
				}
				iz =-1;
			}
			else	appo[iz] =inputString[ix];
		}
		iy++;
		if (ix ==inputString.length())
		{
			appo[iz] ='\0';
			
			if (iy == 3 && !evento)
			{
				if (!strcmp(appo, "APRI"))
					comando =0;
				else
					if (!strcmp(appo, "CHIUDI"))
					comando =1;
				else
					if (!strcmp(appo, "HOME"))
					comando =2;
				else
					if (!strcmp(appo, "AIUTO"))
					comando =3;
				else
					if (!strcmp(appo, "TEST"))
					comando =4;
				else
					if (!strcmp(appo, "TEST_HOME"))
					comando =5;
				else
					if (!strcmp(appo, "TEST_1"))
					comando =6;
				else
					if (!strcmp(appo, "TEST_2"))
					comando =7;
				else
					if (!strcmp(appo, "TEST_3"))
					comando =8;
				else
					if (!strcmp(appo, "TEST_4"))
					comando =9;
				else
					if (!strcmp(appo, "TEST_5"))
					comando =10;
				else
					if (!strcmp(appo, "STATUS"))
					comando =11;

				if (comando != -1)
					Serial.println(inputString);
			}
			else	if (iy == 3 && evento ==1)
			{
				char	workd[11];
				char	worko[3];
				sprintf(workd, "%-4.4s\/%-2.2s\/%-2.2s", appo, &appo[4], &appo[6]);					
				data_agg =workd;
				
				sprintf(worko, "%-2.2s", &appo[8]);					
				ora_agg =worko;
				
				leggi_temperatura();
			}
		}
		inputString = "";
		stringComplete = false;
		if (comando != -1)
		{
			stampa_arduino();
			Serial.print("|allarme|");
			Serial.print("status|");

			pirini =0;
			pirfine =5;
			
			switch(comando)
			{
				case 0:
					Serial.println("ACCESO");
					statusLabel = on;
					stampa_arduino();
					Serial.println("_allarme|ACCESO|.");
					break;
				case 1:
					Serial.println("SPENTO");
					statusLabel = off;
					stampa_arduino();
					Serial.println("_allarme|SPENTO|.");
					break;
				case 2:
					Serial.println("HOME");
					statusLabel = home;
					stampa_arduino();
					Serial.println("_allarme|HOME|.");
					pirfine =3;
					break;
				case 3:
					Serial.println("AIUTO");
					stampa_arduino();
					Serial.println("_allarme|IN ALLARME|Richiesta di AIUTO");
					statusLabel = aiuto;
					break;
				case 4:
					Serial.println("TEST");
					statusLabel = test;
//					Serial.println("arduino_allarme|TEST|.");
					stampa_test("|..");
					break;
				case 5:
					Serial.println("TEST_HOME");
					statusLabel = test_h;
					stampa_test("_HOME|.");
					pirfine =3;
					break;
				case 6:
					Serial.println("TEST_T.sala");
					statusLabel = test_h;
					stampa_test("|Terrazza_sala");
					pirini =0;
					pirfine =1;
					break;
				case 7:
					Serial.println("TEST_F.bagno");
					statusLabel = test_h;
					stampa_test("|Finestra_bagno");
					pirini =1;
					pirfine =2;
					break;
				case 8:
					Serial.println("TEST_T.cucina");
					statusLabel = test_h;
					stampa_test("|Terrazzo_cucina");
					pirini =2;
					pirfine =3;
					break;
				case 9:
					Serial.println("TEST_Z.giorno");
					statusLabel = test_h;
					stampa_test("|Zona_giorno");
					pirini =3;
					pirfine =4;
					break;
				case 10:
					Serial.println("TEST_Z.notte");
					statusLabel = test_h;
					stampa_test("|Zona_notte");
					pirini =4;
					pirfine =5;
					break;	
				case 11:
					if (incorso)
					{
						Serial.println("ALLARME IN CORSO");
						break;
					}
					if(!strcmp(statusLabel, on))
						Serial.println("ACCESO");
					if(!strcmp(statusLabel, home))
						Serial.println("HOME");
					if(!strcmp(statusLabel, off))
						Serial.println("SPENTO");
					if (!strcmp(statusLabel, test))
						Serial.println("TEST");
					if (!strcmp(statusLabel, test_h))
						Serial.println("TEST_parziale");
					break;
			}

		}
	}
  
  	if(!strcmp(statusLabel, on))
	{
		ledStatus = true;
		valled =128;
		analogWrite(ledOin, 128);
		digitalWrite(ledHin, LOW);
	}

  	if(!strcmp(statusLabel, home))	
	{
		ledStatus = true;
		valled =64;
		analogWrite(ledHin, 200);
		digitalWrite(ledOin, LOW);
	}

	if(!strcmp(statusLabel, off)
	|| !strncmp(statusLabel, "TEST", 4))
		ledStatus = false;
	
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
  
	a_status =pirPin =intPin =0;

	for (pir =pirini; pir <pirfine; pir++)
	{
		p_status[pir] = digitalRead(pin_pir[pir]);
		if (!pirPin && p_status[pir])
			pirPin =pir +1;
	}
	if ((inizio_pir && fine_pir && pirPin && !incorso)
	|| (inizio_pir && !fine_pir && (pirPin != pir_inizio) && !incorso))
	{
		stampa_arduino();
		Serial.print("_allarme\|Allarme scattato 2' impulso\|");      //output
		Serial.println(millis() -inizio_pir);
		a_status =pirPin;
	}
	else	if (pirPin && !inizio_pir && !incorso)
	{
		inizio_pir =millis();
		pir_inizio =pirPin;
		// Serial.print("Pir -> ");      //output
		// Serial.println(pirPin);
	}
	
	if (inizio_pir && !incorso && pirPin
	&& ((millis() -tempoallarme) >=inizio_pir))
	{
		stampa_arduino();
		Serial.print("_allarme\|Allarme scattato\|");      //output
		Serial.println(millis() -inizio_pir);
		inizio_pir =fine_pir =0L;
		a_status =pirPin;
	}        
	if (!pirPin && inizio_pir && !fine_pir)
		fine_pir =millis();
// Controllo integrita' fili pir esterni	
	for (pir =0; pir <2; pir++)
	{
		i_status[pir] = digitalRead(pin_int[pir]);
		if (!intPin && !i_status[pir])
			intPin =pir +1;
	}
	
	for (pir =0; pir <2 && !incorso; pir++)
	{
		if ((!strcmp(statusLabel, on)
			|| !strcmp(statusLabel, home)
			|| !strncmp(statusLabel, "TEST", 4))
		&& (i_status[pir] !=i_stapre[pir]))
		{
			stampa_arduino();
			Serial.print("|");
			switch(pir +1)
			{
				case 1:
					sprintf(appo, "%s", "Finestra_bagno");
					break;
				case 2:
					sprintf(appo, "%s", "Terrazzo_cucina");
					break;
			}
			Serial.print(appo);
			Serial.print("|status|");
			if (i_status[pir])
				Serial.println("filo_integro");
			else
				Serial.println("filo_interrotto");
		}
		i_stapre[pir] =i_status[pir];
	}
	
		
	for (pir =pirini; pir <pirfine && !incorso; pir++)
	{
		if ((!strcmp(statusLabel, on)
			|| !strcmp(statusLabel, home)
			|| !strncmp(statusLabel, "TEST", 4)
			|| p_stapre[pir] <0)
		&& (p_status[pir] !=p_stapre[pir]))
		{
			stampa_arduino();
			Serial.print("|");
			switch(pir +1)
			{
				case 1:
					sprintf(appo, "%s", "Terrazzo_sala");
					break;
				case 2:
					sprintf(appo, "%s", "Finestra_bagno");
					break;
				case 3:
					sprintf(appo, "%s", "Terrazzo_cucina");
					break;
				case 4:
					sprintf(appo, "%s", "Zona_giorno");
					break;
				case 5:
					sprintf(appo, "%s", "Zona_notte");
					break;
			}
			Serial.print(appo);
			Serial.print("|status|");
			if (p_status[pir])
				Serial.println("movimento");
			else
				Serial.println("attesa");
		}
		p_stapre[pir] =p_status[pir];
	}
	if (!strcmp(statusLabel, home))
	{
		for (pir =3; pir <5 && !incorso; pir++)
		{
			if (p_status[pir] !=p_stapre[pir])
			{
				stampa_arduino();
				Serial.print("|");
				switch(pir +1)
				{
					case 4:
						sprintf(appo, "%s", "Zona_giorno");
						break;
					case 5:
						sprintf(appo, "%s", "Zona_notte");
						break;
				}
				Serial.print(appo);
				Serial.print("|status|");
				if (p_status[pir])
					Serial.println("movimento");
				else
					Serial.println("attesa");
			}
			p_stapre[pir] =p_status[pir];
		}
	}
	if (!incorso
	&& (stato != a_status || intPin))
	{
		if (intPin)
			stato =intPin;
		else
			stato =a_status;
		
		if (stato)
		{
			int i;
			
			*appo ='\0';
		
			if (!intPin)
				switch(pirPin)
			{
				case 1:
				sprintf(appo, "%s", "PIR -> Terrazza_sala");
				break;
				case 2:
				sprintf(appo, "%s", "PIR -> Finestra_bagno");
				break;
				case 3:
				sprintf(appo, "%s", "PIR -> Terrazzo_cucina");
				break;
				case 4:
				sprintf(appo, "%s", "PIR -> Zona_giorno");
				break;
				case 5:
				sprintf(appo, "%s", "PIR -> Zona_notte");
				break;
			}

			switch(intPin)
			{
				case 1:
					sprintf(appo, "%s", "Filo -> Terrazza_sala");
					break;
				case 2:
					sprintf(appo, "%s", "Filo -> Finestra_bagno");
					break;
				case 3:
					sprintf(appo, "%s", "Filo -> Terrazzo_cucina");
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
				stampa_arduino();
				Serial.println("|allarme|status|ALLARME IN CORSO");
				stampa_arduino();
				Serial.print("_allarme|IN ALLARME|");
				Serial.println(appo);
			}
			else	if(!incorso
				&& !strncmp(statusLabel, "TEST", 4))
			{
				inizio_all =millis();
				incorso =1;
				stampa_arduino();
				Serial.print("_allarme|IN TEST|");
				Serial.println(appo);
			}
		}
		else  Serial.println(F("Spento"));
	}

	if (millis() <inizio_pir
	|| (fine_pir && ((millis() -fine_pir) >tempoallarme)))
	{
		// Serial.println("inizio - ");
		// Serial.println(inizio_pir);
		// Serial.println(tempoallarme);
		// Serial.println(millis());
		// Serial.println(fine_pir);
		inizio_pir = fine_pir =0L;
	}
}

void serialEvent()
{
	while (Serial.available())
	{
		// get the new byte:
		char inChar = (char)Serial.read();
		// if the incoming character is a newline, set a flag
		// so the main loop can do something about it:
		if (inChar == '\n' || inChar == '\r')
		{
			inputString += "";
			stringComplete = true;
		}
		else	inputString += inChar;	// add it to the inputString:
	}
}

void leggi_temperatura()
{
	//
	// Reading temperature or humidity takes about 250 milliseconds!
	// Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
	float h = dht1.readHumidity();
	float t = dht1.readTemperature();
	float hic;
	if (h <40.0 || t <27.0)
		hic =t;
	else
		hic = dht1.computeHeatIndex(t, h, false);

// check if returns are valid, if they are NaN (not a number) then something went wrong!
	if (isnan(t) || isnan(h))
	{
		Serial.println("Failed to read from DHT #1");
	}
	else
	{
		stampa_temp("cucina", t, h, hic);
		// Serial.print("arduino|");
		// Serial.print("cucina|");
		// Serial.print("temperatura|");
		// Serial.println(t);
		// Serial.print("arduino|");
		// Serial.print("cucina|");
		// Serial.print("umidita|");
		// Serial.println(h);
		// Serial.print("arduino|");
		// Serial.print("cucina|");
		// Serial.print("percepita|");
		// Serial.println(hic);
		// Serial.print("arduino_temp|");
		// Serial.print(data_agg);
		// Serial.print("|");
		// Serial.print(ora_agg);
		// Serial.print("|temp_cucina|");
		// Serial.println(t);
		// Serial.print("arduino_temp|");
		// Serial.print(data_agg);
		// Serial.print("|");
		// Serial.print(ora_agg);
		// Serial.print("|umid_cucina|");
		// Serial.println(h);
		// Serial.print("arduino_temp|");
		// Serial.print(data_agg);
		// Serial.print("|");
		// Serial.print(ora_agg);
		// Serial.print("|perc_cucina|");
		// Serial.println(hic);
	}
	h = dht2.readHumidity();
	t = dht2.readTemperature();
	
	if (h <40.0 || t <27.0)
		hic =t;
	else
		hic = dht2.computeHeatIndex(t, h, false);
	
	if (isnan(t) || isnan(h))
	{
		Serial.println("Failed to read from DHT #2");
	}
	else
	{
		stampa_temp("esterna", t, h, hic);
		// Serial.print("arduino|");
		// Serial.print("terrazzo_cucina|");
		// Serial.print("temperatura|");
		// Serial.println(t);
		// Serial.print("arduino|");
		// Serial.print("terrazzo_cucina|");
		// Serial.print("umidita|");
		// Serial.println(h);
		// Serial.print("arduino|");
		// Serial.print("terrazzo_cucina|");
		// Serial.print("percepita|");
		// Serial.println(hic);
		// Serial.print("arduino_temp|");
		// Serial.print(data_agg);
		// Serial.print("|");
		// Serial.print(ora_agg);
		// Serial.print("|temp_esterna|");
		// Serial.println(t);
		// Serial.print("arduino_temp|");
		// Serial.print(data_agg);
		// Serial.print("|");
		// Serial.print(ora_agg);
		// Serial.print("|umid_esterna|");
		// Serial.println(h);
		// Serial.print("arduino_temp|");
		// Serial.print(data_agg);
		// Serial.print("|");
		// Serial.print(ora_agg);
		// Serial.print("|perc_esterna|");
		// Serial.println(hic);
	}
	h = dht3.readHumidity();
	t = dht3.readTemperature();

	if (h <40.0 || t <27.0)
		hic =t;
	else
		hic = dht3.computeHeatIndex(t, h, false);
	
	if (isnan(t) || isnan(h))
	{
		Serial.println("Failed to read from DHT #3");
	}
	else
	{
		stampa_temp("studio", t, h, hic);
	}
	h = dht4.readHumidity();
	t = dht4.readTemperature();
	
	if (h <40.0 || t <27.0)
		hic =t;
	else
		hic = dht4.computeHeatIndex(t, h, false);
	
	if (isnan(t) || isnan(h))
	{
		Serial.println("Failed to read from DHT #4");
	}
	else
	{
		stampa_temp("camera", t, h, hic);
	}
}

void	stampa_temp(char *stanza, float temp, float umid, float perc)
{
	stampa_arduino();
	Serial.print("|");
	Serial.print(stanza);
	Serial.print("|temperatura|");
	Serial.println(temp);
	stampa_arduino();
	Serial.print("|");
	Serial.print(stanza);
	Serial.print("|umidita|");
	Serial.println(umid);
	stampa_arduino();
	Serial.print("|");
	Serial.print(stanza);
	Serial.print("|percepita|");
	Serial.println(perc);
	stampa_arduino();
	Serial.print("_temp|");
	Serial.print(data_agg);
	Serial.print("|");
	Serial.print(ora_agg);
	Serial.print("|temp_");
	Serial.print(stanza);
	Serial.print("|");
	Serial.println(temp);
	stampa_arduino();
	Serial.print("_temp|");
	Serial.print(data_agg);
	Serial.print("|");
	Serial.print(ora_agg);
	Serial.print("|umid_");
	Serial.print(stanza);
	Serial.print("|");
	Serial.println(umid);
	stampa_arduino();
	Serial.print("_temp|");
	Serial.print(data_agg);
	Serial.print("|");
	Serial.print(ora_agg);
	Serial.print("|perc_");
	Serial.print(stanza);
	Serial.print("|");
	Serial.println(perc);
}
void	stampa_test(char *stringa)
{
	Serial.print("arduino_allarme|TEST");
	Serial.println(stringa);
}
void	stampa_arduino()
{
	Serial.print("arduino");
}
