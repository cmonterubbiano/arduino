/* Tratto dall'esempio "Simply TinyGPS" di Mikal Hart
   e modificato da Simone Dall'Asta per integrare
   l'inserimento in scheda SD e aggiunta di misurazione
   di altitudine e velocità.

   http://www.dallaf.it/simo
*/

#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
#include <TinyGPS.h>

TinyGPS gps;
SoftwareSerial ss(8,7);
File file;
const int buttonPin = 6;     // the number of the pushbutton pin
const int ledPin1 =  9;      // the number of the LED pin
const int ledPin2 =  3;      // the number of the LED pin
long	p_tempo =0L;
byte	stato_pre =0;
byte	stato_att =0;
char	coord[15];
long	ultdat =0L;
long	ulttim =0L;
float	ultlat;
int		disattivo =0;
int		errore =0;

void setup()
{
	pinMode(ledPin1, OUTPUT);
	pinMode(ledPin2, OUTPUT);
	pinMode(buttonPin, INPUT);
	digitalWrite(buttonPin, HIGH);
	digitalWrite(ledPin1, HIGH);
	digitalWrite(ledPin2, HIGH);
	*coord ='\0';
	Serial.begin(9600);
	ss.begin(9600);

	Serial.print("Card: ");
	if (!SD.begin(4)) //il Pin 7 è collegato a CS
	{
		Serial.println("KO!");
		return;
	}
	Serial.println("OK");
	digitalWrite(ledPin1, LOW);

	Serial.print("TinyGPS v. ");
	Serial.println(TinyGPS::library_version());
	Serial.println();
}

void loop()
{
	bool newData = false;
	unsigned long chars;
	unsigned short sentences, failed;
	char	appo[40];
	int  pulstat =digitalRead(buttonPin);
		
	if (!pulstat)
	{
		if (!p_tempo)
			p_tempo =millis();
		else
		{
			
			ultdat =0L;
			if (!disattivo)
				disattivo =1;		
			stato_att =(!stato_att ? 1 : 0);
		}
	}
	else	if(p_tempo)
		disattivo =p_tempo =0;		

	if (stato_att !=stato_pre)
	{
		*coord ='\0';
		stato_pre =stato_att;
		if (!stato_att)
			digitalWrite(ledPin2, HIGH);
		else
			digitalWrite(ledPin2, LOW);
	}
  // For one second we parse GPS data and report some key values
	for (unsigned long start = millis(); millis() - start < 1000;)
	{
		while (ss.available())
		{
			char c = ss.read();
//			Serial.write(c); // uncomment this line if you want to see the GPS data flowing
			if (gps.encode(c)) // Did a new valid sentence come in?
				newData = true;
		}
	}

	if (newData)
	{
		float flat, flon;
		unsigned long data_ddmmyy, time_hhmmsscc, age_ms;
		float speed_kmh = 0;
		float altitude_m;
		unsigned long age;
		unsigned long datatt;
		
		gps.f_get_position(&flat, &flon, &age);
		gps.get_datetime(&data_ddmmyy, &time_hhmmsscc, &age_ms);
		speed_kmh = gps.f_speed_kmph();
		altitude_m = gps.f_altitude();
		
		datatt =(data_ddmmyy %100L) *10000L +((data_ddmmyy /100L) %100L) *100L +(data_ddmmyy /10000L);

		if (datatt <ultdat
		|| (datatt ==ultdat && time_hhmmsscc <ulttim))
			errore++;
		else
			if (ultdat)
		{
			// dato x scontato che alla velocita' di 1200 KMH in 1 secondo
			// si percorrono 0,003 gradi di latitudine
			// se la differenza e' maggiore le coodinate vengono scartate
			float difflat =(flat * 1000.0)-(ultlat * 1000.0);
			unsigned long	secatt;
			unsigned long	secpre;
			
			if (difflat <.0)
				difflat *= -1.0;
			secatt =((time_hhmmsscc /1000000L) * 3600L) + (((time_hhmmsscc /10000L) %100L)* 60L) + ((time_hhmmsscc /100L) %100L) + ((datatt - ultdat) * 24L * 3600L);
			secpre =(ulttim /1000000L) * 3600L + ((ulttim /10000L) %100L)* 60L + ((ulttim /100L) %100L);
			// Serial.println(difflat);
			Serial.println(secatt -secpre);
			if ((difflat /(secatt -secpre)) >3.0)
				errore++;
			else
				errore =0;
		}
		else
			errore =0;
		
		if (errore >50)
			errore =0;

		String risultato_gps = "";

		if (age != TinyGPS::GPS_INVALID_FIX_TIME)
		{
			if (!*coord && stato_att)
				sprintf(coord, "%02ld%02ld%04ld", ((data_ddmmyy /100L) %100L), (data_ddmmyy /10000L),(time_hhmmsscc /10000L));

			risultato_gps += "[";
			sprintf(appo, "%02ld/%02ld/%02ld", (data_ddmmyy /10000L), ((data_ddmmyy /100L) %100L), (data_ddmmyy %100L));
			risultato_gps += appo;
			risultato_gps += " alle ";
			sprintf(appo, "%02ld:%02ld:%02ld", (time_hhmmsscc /1000000L), ((time_hhmmsscc /10000L) %100L), ((time_hhmmsscc /100L) %100L));
			risultato_gps += appo;
			risultato_gps += " ]  ";
		}
		long  l_flat =flat *10000000L;
		long  l_flon =flon *10000000L;
		long  l_alti =altitude_m *100L;

		risultato_gps += "LAT=";
		risultato_gps += l_flat /10000000L;
		risultato_gps += ".";
		risultato_gps +=(l_flat <0L ? (l_flat %10000000L * -1L) : (l_flat %10000000L));
		risultato_gps += "-LON=";
		risultato_gps += l_flon /10000000L;
		risultato_gps += ".";
		risultato_gps +=(l_flon <0L ? (l_flon %10000000L * -1L) : (l_flon %10000000L));
		risultato_gps += "-SAT=";
		risultato_gps += gps.satellites();
		risultato_gps += "-PREC=";
		risultato_gps += gps.hdop();
		risultato_gps += "-VEL=";
		risultato_gps += speed_kmh;
		// risultato_gps += "km/h";
		risultato_gps += "-ALT=";
		risultato_gps += altitude_m;
		// risultato_gps += "m s.l.m.";

		if (*coord
		and age != TinyGPS::GPS_INVALID_FIX_TIME
		and flat != TinyGPS::GPS_INVALID_F_ANGLE
		and flon != TinyGPS::GPS_INVALID_F_ANGLE
		and gps.satellites() != TinyGPS::GPS_INVALID_SATELLITES
		and gps.hdop() != TinyGPS::GPS_INVALID_HDOP)
		{
			// digitalWrite(ledPin2, LOW);
			sprintf(appo, "%s.%s", coord, (!errore ? "COO" : "ERC"));
			file = SD.open(appo, FILE_WRITE); //File in scrittura
			if (file) //Se il file è stato aperto correttamente
			{
				Serial.println(risultato_gps);
        // Serial.println("Scrittura su file: ");
				file.println(risultato_gps); //Scrivo su file il numero
				file.close(); //Chiusura file
			}
			else
			{
				Serial.println("ERR: coord");
			}
			sprintf(appo, "%s.%s", coord, (!errore ? "GAR" : "ERG"));

			file = SD.open(appo, FILE_WRITE); //File in scrittura
			
			if (file) //Se il file Ã¨ stato aperto correttamente
			{
		//<trkpt lat="43.299956" lon="13.735479">
				sprintf(appo, "\t\t<trkpt lat=\"%02ld.%07ld\" lon=\"%02ld.%07ld\">", (l_flat /10000000L), (l_flat <0L ? (l_flat %10000000L * -1L) : (l_flat %10000000L)), (l_flon /10000000L), (l_flon <0L ? (l_flon %10000000L * -1L) : (l_flon %10000000L)));
				Serial.println(appo);
				file.println(appo); //Scrivo su file il numero
		//<ele>664.20001220703125</ele>
				sprintf(appo, "\t\t\t<ele>%01ld.%02ld</ele>", (l_alti /100L), (l_alti %100L));
				Serial.println(appo);
				file.println(appo); //Scrivo su file il numero
		//<time>2017-01-09T09:47:58.000Z</time>
		// sprintf(appo, "\t\t\t<time>20%02ld-%02ld-%02ldT%02ld:%02ld:%02ld.%02ldZ</time>", (data_ddmmyy %100L), ((data_ddmmyy /100L) %100L), (data_ddmmyy /10000L), (time_hhmmsscc /1000000L), ((time_hhmmsscc /10000L) %100L), ((time_hhmmsscc /100L) %100L), (time_hhmmsscc %100L));
				sprintf(appo, "\t\t\t<time>20%02ld-%02ld-%02ld", (data_ddmmyy %100L), ((data_ddmmyy /100L) %100L), (data_ddmmyy /10000L));
				risultato_gps = "";
				risultato_gps +=appo;
				risultato_gps +="T";
				sprintf(appo, "%02ld", (time_hhmmsscc /1000000L));
				risultato_gps +=appo;
				risultato_gps +=":";
				sprintf(appo, "%02ld", ((time_hhmmsscc /10000L) %100L));
				risultato_gps +=appo;
				risultato_gps +=":";
				sprintf(appo, "%02ld", ((time_hhmmsscc /100L) %100L));
				risultato_gps +=appo;
				risultato_gps +=".";
				sprintf(appo, "%02ld", (time_hhmmsscc %100L));
				risultato_gps +=appo;
				risultato_gps +="Z</time>";
		// risultato_gps +="Z";
		// risultato_gps +="</time>";
				Serial.println(risultato_gps);
				file.println(risultato_gps); //Scrivo su file il numero
		//</trkpt>
				strcpy(appo, "\t\t</trkpt>");
				Serial.println(appo);
				file.println(appo); //Scrivo su file il numero

				file.close(); //Chiusura file
			}
			else
				Serial.println("ERR: garmin");

			if (!errore)
			{
				ultdat =datatt;
				ulttim =time_hhmmsscc;
				ultlat =flat;
			}
		}
		risultato_gps = "";
	}

//Lo sketch usa 24.558 byte (76%) dello spazio disponibile per i programmi. Il massimo è 32.256 byte.
//Le variabili globali usano 1.563 byte (76%) di memoria dinamica,
//Lo sketch usa 24.606 byte (76%) dello spazio disponibile per i programmi. Il massimo è 32.256 byte.
//Le variabili globali usano 1.597 byte (77%) di memoria dinamica, lasciando altri 451 byte liberi per le variabili
	gps.stats(&chars, &sentences, &failed);
  // Serial.print(" CHARS=");
  // Serial.print(chars);
  // Serial.print(" SENTENCES=");
  // Serial.print(sentences);
  // Serial.print(" CSUM ERR=");
  // Serial.println(failed);
	if (chars == 0)
		Serial.println("** No charfrom GPS: check wiring **");
	delay(1000);
}
