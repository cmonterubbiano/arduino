// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"

#define DHTPIN0 2     // what pin we're connected to
#define DHTPIN1 3     // what pin we're connected to
#define DHTPIN2 4     // what pin we're connected to
#define DHTPIN3 5     // what pin we're connected to
#define DHTPIN4 6     // what pin we're connected to
#define DHTPIN5 7     // what pin we're connected to

//  Legenda   camos[p12] = camera ospiti
//            cambam     = camera bambini
//            camgio     = camera Giorgia
//            salone    = salone
//            studio    = studio
//            bagnop    = bagno piccolo
/*
#define F_camos1 20   // Sensore finestra 1  1 =est 2 = ovest
#define F_camos2 22   // Sensore finestra 2  1 =est 2 = ovest
#define F_bagnog 24   // Sensore finestra
#define F_studio 26   // Sensore finestra
#define F_cambam 28   // Sensore finestra
#define F_camgio 30   // Sensore finestra
#define F_salone 32   // Sensore finestra

#define S_camos1 21   // Sensore serranda 1  1 =est 2 = ovest
#define S_camos2 23   // Sensore serranda 2  1 =est 2 = ovest
#define S_bagnog 25   // Sensore serranda

#define S_cambam 29   // Sensore serranda
#define S_camgio 31   // Sensore serranda

#define P_ingres 33   // Sensore portone
#define P_giardi 27   // Sensore portone
*/
#define T_MAX 40000   // Tempo massimo apertura/chiusura serranda
#define P_MIN 200     // Tempo minimo di pressione pulsante in millisecondi
#define A_MAX 1000    // Tempo massimo x indicare chiusura serranda sopra tale tempo apertura


byte  n_senrele[] ={  7,	//numero sensori finestre
					  5,	//numero sensori serrande
					  2,	//numero sensoro portoni
					  5,	//numero rele apertura
					  5,	//numero rele chiusura
					  2};	//numero rele riscaldamento
byte  f_sensori	[] ={  20, 22, 24, 26, 28, 30, 32};	//sensori finestre
byte  s_sensori	[] ={  21, 23, 25, 29, 31};			//sensori serrande
byte  p_sensori	[] ={  33, 27};						//sensori portoni
byte  a_rele	[] ={  40, 42, 44, 46, 48};			//rele    apertura
byte  a_roff	[] ={  1,  1,  1,  1,  1};			//Status off rele
byte  c_rele	[] ={  41, 43, 45, 47, 49};			//rele    chiusura
byte  c_roff	[] ={  1,  1,  1,  1,  1};			//Status off rele
byte  r_rele	[] ={  50, 51};						//rele    riscaldamento
byte  r_roff	[] ={  1,  1};						//Status off rele
byte  f_status	[] ={  0, 0, 0, 0, 0, 0, 0};		//status sensori 0 chiuso 1 aperto
byte  s_status	[] ={  0, 0, 0, 0, 0};
byte  p_status	[] ={  0, 0};
byte  r_status	[] ={  0, 0};						//status riscaldamento 0 spento 1 acceso
byte  a_status	=0;									//status allarme    0 spento 1 acceso
byte  te_status	=0;									//status telecamera 0 spenta 1 accesa
byte  p1_status	=0;									//status presa 1    0 spenta 1 accesa
byte  f_stapre	[] ={  -1, 0, 0, 0, 0, 0, 0};		//status sensori 0 chiuso 1 aperto
byte  s_stapre	[] ={  -1, 0, 0, 0, 0, 0};
byte  p_stapre	[] ={  -1, 0};
byte  r_stapre	[] ={  -1, -1};						//status riscaldamento 0 spento 1 acceso
byte  a_stapre	=-1;
byte  te_stapre	=-1;									//status telecamera 0 spenta 1 accesa
byte  p1_stapre	=-1;									//status presa 1    0 spenta 1 accesa
byte  f_staall	[] ={  -1, 0, 0, 0, 0, 0, 0};		//status sensori 0 chiuso 1 aperto
byte  s_staall	[] ={  -1, 0, 0, 0, 0, 0};
byte  p_staall	[] ={  -1, 0};						//status allarme  0 spento 1 acceso
unsigned  long  a_tempo   [] ={  0, 0, 0, 0, 0, 0};	//rele     apertura tempo iniziale apertura
unsigned  long  c_tempo   [] ={  0, 0, 0, 0, 0, 0};	//rele     chiusura tempo iniziale chisura
unsigned  long  p_tempo   [] ={  0, 0, 0, 0, 0, 0};	//pulsante chiusura tempo iniziale chisura
unsigned  long  t_ultlet;							// tempo ultima lettura
/*
#define A_camos1 40   // Apertura serranda 1  1 =est 2 = ovest
#define A_camos2 42   // Apertura serranda 2  1 =est 2 = ovest
#define A_bagnog 44   // Apertura serranda
#define A_cambam 46   // Apertura serranda
#define A_camgio 48   // Apertura serranda

#define C_camos1 41   // Chiusura serranda 1  1 =est 2 = ovest
#define C_camos2 43   // Chiusura serranda 2  1 =est 2 = ovest
#define C_bagnog 45   // Chiusura serranda
#define C_cambam 47   // Chiusura serranda
#define C_camgio 49   // Chiusura serranda
*/
//  Pseudo comandi simulati con pulsanti in mancanza del web
byte s_pulsanti []= { 8,  9,  10, 11,  12, 13};
/* 
#define W_camos1 8    // Comando serranda 1  1 =est 2 = ovest
#define W_camos2 9    // Comando serranda 2  1 =est 2 = ovest
#define W_bagnog 10   // Comando serranda
#define W_cambam 11   // Comando serranda
#define W_camgio 12   // Comando serranda
*/
// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT t_camosp(DHTPIN0, DHTTYPE);
DHT t_cambam(DHTPIN1, DHTTYPE);
DHT t_camgio(DHTPIN2, DHTTYPE);
DHT t_salone(DHTPIN3, DHTTYPE);
DHT t_studio(DHTPIN4, DHTTYPE);
DHT t_bagnop(DHTPIN5, DHTTYPE);

float  t_min   [] ={  1000, 1000, 1000, 1000, 1000, 1000};    //temperatura minima
float  t_max   [] ={  -1000, -1000, -1000, -1000, -1000, -1000};    //temperatura mamssima
float  t_ult   [] ={  0, 0, 0, 0, 0, 0};    //temperatura ultima

float  u_min   [] ={  1000, 1000, 1000, 1000, 1000, 1000};    //umidita minima
float  u_max   [] ={  -1000, -1000, -1000, -1000, -1000, -1000};    //umidita mamssima
float  u_ult   [] ={  0, 0, 0, 0, 0, 0};    //umidita ultima

int	  s_ult =0;	// numero sensore da leggere

String	inputString = "";         // a string to hold incoming data
boolean	stringComplete = false;  // whether the string is complete

void setup() 
{
  int ix ;
  int iy;

  t_ultlet =0L;
  Serial.begin(9600);
  // Serial.println("DHTxx test!");

  t_camosp.begin();
  t_cambam.begin();
  t_camgio.begin();
  t_salone.begin();
  t_studio.begin();
  t_bagnop.begin();


  for (ix =iy =0; ix <n_senrele[iy]; ix++)
  {
	pinMode(f_sensori[ix], INPUT);
	digitalWrite(f_sensori[ix], HIGH);
  }
  iy++;
  for (ix =0; ix <n_senrele[iy]; ix++)
  {
	pinMode(s_sensori[ix], INPUT);
	digitalWrite(s_sensori[ix], HIGH);
	delay(20);
  }
  iy++;
  for (ix =0; ix <n_senrele[iy]; ix++)
  {
	pinMode(p_sensori[ix], INPUT);
	digitalWrite(p_sensori[ix], HIGH);
	delay(20);
  }
  iy++;
  for (ix =0; ix <n_senrele[iy]; ix++)
  {
	pinMode(a_rele[ix], OUTPUT);
	delay(20);
	digitalWrite(a_rele[ix], (!a_roff[ix] ? LOW : HIGH));
	delay(20);
  }
  iy++;
  for (ix =0; ix <n_senrele[iy]; ix++)
  {
	pinMode(c_rele[ix], OUTPUT);
	delay(20);
	digitalWrite(c_rele[ix], (!c_roff[ix] ? LOW : HIGH));
	delay(20);
  }
  iy++;
  for (ix =0; ix <n_senrele[iy]; ix++)
  {
	pinMode(r_rele[ix], OUTPUT);
	delay(20);
	digitalWrite(r_rele[ix], (!r_roff[ix] ? LOW : HIGH));
	delay(20);
  }
  for (ix =0; ix <6; ix++)
  {
	pinMode(s_pulsanti[ix], INPUT);
	digitalWrite(s_pulsanti[ix], HIGH);
	delay(20);
  }
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}

void loop() 
{
	int  pulsa;
	int  ix;
	int  iy;
  
	serialEvent(); //call the function
	// print the string when a newline arrives:
	if (stringComplete)
	{
		int ix;
		int iy;
		int	iz;
		char	appo[200];
		int		stanza =-1;
		int		serranda =-1;
		int		comando =-1;
		
		for(ix =iy =iz =0; ix <inputString.length(); ix++, iz++)
		{
			//	raspberry|camera_giorgia|serranda|APRI
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
					if (!strcmp(appo, "camera_ospiti"))
						stanza =0;
					else
						if (!strcmp(appo, "bagno_grande"))
						stanza =2;
					else
						if (!strcmp(appo, "camera_bambini"))
						stanza =3;
					else
						if (!strcmp(appo, "camera_giorgia"))
						stanza =4;
					else
						if (!strcmp(appo, "tutte"))
						stanza =5;
					else
						if (!strcmp(appo, "riscaldamento"))
						stanza =6;
					else
						if (!strcmp(appo, "allarme"))
						stanza =7;
					else
						if (!strcmp(appo, "telecamera"))
						stanza =8;
					else
						if (!strcmp(appo, "presa_1"))
						stanza =9;
					
					if (stanza <0)
						break;
				}
				if (iy == 3)
				{
					if ((!strcmp(appo, "serranda") && stanza <6)
					|| (!strcmp(appo, "giorno") && stanza ==6)
					|| stanza >6)
						serranda =0;
					else
						if ((!strcmp(appo, "serranda_1") && stanza <6)
						|| (!strcmp(appo, "notte") && stanza ==6))
						serranda =1;
					
					if (serranda <0)
						break;
				}

				iz =-1;
			}
			else	appo[iz] =inputString[ix];
		}
		iy++;
		if (ix ==inputString.length())
		{
			appo[iz] ='\0';
			
			if (iy == 4)
			{
				if (!strcmp(appo, "APRI"))
					comando =0;
				else
					if (!strcmp(appo, "CHIUDI"))
					comando =1;
				else
					if (!strcmp(appo, "FERMA"))
					comando =2;
				if (comando != -1)
					Serial.println(inputString);
			}
		}
		// clear the string
		inputString = "";
		stringComplete = false;
		if (stanza != -1 && serranda != -1 && comando != -1)
		{
			if ((stanza <6 && !serranda)
			|| (stanza <6 && (serranda && !stanza)))
			{
				if (comando ==2 && s_stapre[(stanza +serranda)] ==2)
						p_tempo[(stanza +serranda)] =(millis() -300);
				else	if (comando ==1)
						p_tempo[(stanza +serranda)] =(millis() -300);
				else	if (!comando)
						p_tempo[(stanza +serranda)] =(millis() -1010);
			}
			else
				if (stanza >5)
			{
				if (stanza ==6 && !comando)
						r_status[serranda] =1;
				else	if (stanza ==6 && comando)
						r_status[serranda] =0;
				else	if (stanza ==7 && !comando)
						a_status =1;
				else	if (stanza ==7 && comando)
						a_status =0;
				else	if (stanza ==8 && !comando)
						te_status =1;
				else	if (stanza ==8 && comando)
						te_status =0;
				else	if (stanza ==9 && !comando)
						p1_status =1;
				else	if (stanza ==9 && comando)
						p1_status =0;
			}
		}
	}
	
	if (!t_ultlet
	|| ((millis() -t_ultlet) >=10000L))
	{
		int	diverso =0;
		float h;
		float t;
		float hic;
		
		t_ultlet =millis();
		// Serial.print("Inizio: ");
		// Serial.println(t_ultlet);
		// Reading temperature or humidity takes about 60 conds!
		// Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
		switch (s_ult)
		{
			case 0:
				h = t_camosp.readHumidity();
				t = t_camosp.readTemperature();
				break;
			case 1:
				h = t_cambam.readHumidity();
				t = t_cambam.readTemperature();
				break;
			case 2:
				h = t_camgio.readHumidity();
				t = t_camgio.readTemperature();
				break;
			case 3:
				h = t_salone.readHumidity();
				t = t_salone.readTemperature();
				break;
			case 4:
				h = t_studio.readHumidity();
				t = t_studio.readTemperature();
				break;
		}
		
		if (h !=u_ult[s_ult]
		&& (!isnan(h) || !isnan(u_ult[s_ult])))
		{
			// Serial.print("h -> ");
			// Serial.print(h);
			// Serial.print(" ult -> ");
			// Serial.print(u_ult[s_ult]);
			// Serial.print("|");
			Serial.print("arduino|");
			
			switch (s_ult)
			{
				case 0:
					Serial.print("camera_ospiti|");;
					break;
				case 1:
					Serial.print("camera_bambini|");
					break;
				case 2:
					Serial.print("camera_giorgia|");
					break;
				case 3:
					Serial.print("sala|");
					break;
				case 4:
					Serial.print("studio|");
					break;
			}
			Serial.print("umidita|");
			Serial.println(h);
		}
		
		if (t !=t_ult[s_ult]
		&& (!isnan(t) || !isnan(t_ult[s_ult])))
		{
			Serial.print("arduino|");
			
			switch (s_ult)
			{
				case 0:
					Serial.print("camera_ospiti|");;
					break;
				case 1:
					Serial.print("camera_bambini|");
					break;
				case 2:
					Serial.print("camera_giorgia|");
					break;
				case 3:
					Serial.print("sala|");
					break;
				case 4:
					Serial.print("studio|");
					break;
			}
			Serial.print("temperatura|");
			Serial.println(t);
		}
		
		if ((t !=t_ult[s_ult] || h !=u_ult[s_ult])
		&& (!isnan(t) || !isnan(t_ult[s_ult]) || !isnan(h) || !isnan(u_ult[s_ult])))
		{
			Serial.print("arduino|");
			
			switch (s_ult)
			{
				case 0:
					Serial.print("camera_ospiti|");
					hic = t_camosp.computeHeatIndex(t, h, false);
					break;
				case 1:
					Serial.print("camera_bambini|");
					hic = t_cambam.computeHeatIndex(t, h, false);
					break;
				case 2:
					Serial.print("camera_giorgia|");
					hic = t_camgio.computeHeatIndex(t, h, false);
					break;
				case 3:
					Serial.print("sala|");
					hic = t_salone.computeHeatIndex(t, h, false);
					break;
				case 4:
					Serial.print("studio|");
					hic = t_studio.computeHeatIndex(t, h, false);
					break;
			}
			Serial.print("temperatura_percepita|");
			Serial.println(hic);
		}
		if (u_min[s_ult]  > h)
			u_min[s_ult] =h;
		if (u_max[s_ult] < h)
			u_max[s_ult] =h;
		u_ult[s_ult] =h;
		if (t_min[s_ult]  > t)
			t_min[s_ult] =t;
		if (t_max[s_ult] < t)
			t_max[s_ult] =t;
		t_ult[s_ult] =t;

		// Check if any reads failed and exit early (to try again).
		// if (isnan(h) || isnan(t) || isnan(hic))
			// Serial.println("Failed to read from t_camosp sensor!");
		// else
		// {
			// Serial.print("Umidita'-0-: ");
			// Serial.print(h);
			// Serial.print(" %|");
			// Serial.print("Temperatura: ");
			// Serial.print(t);
			// Serial.print(" *C|");
			// Serial.print("Percepita: ");
			// Serial.print(hic);
			// Serial.print(" *C ");
			// Serial.print("|");
			// Serial.print("Umidita' min: ");
			// Serial.print(u_min[s_ult]);
			// Serial.print("|");
			// Serial.print(" max: ");
			// Serial.print(u_max[s_ult]);
			// Serial.print("|");
			// Serial.print("Temperatura min: ");
			// Serial.print(t_min[s_ult]);
			// Serial.print("|");
			// Serial.print(" max: ");
			// Serial.println(t_max[s_ult]);
		// }
		s_ult++;
		if (s_ult >4)
			s_ult =0;
		
		for (ix =iy =0; ix <n_senrele[iy]; ix++)
		{
			f_status[ix] =digitalRead(f_sensori[ix]);
			if (f_stapre[ix] !=f_status[ix])
			{
				diverso =1;
				f_stapre[ix] =f_status[ix];
			}
		}
		if (diverso)
		{
			diverso =0;
			for (ix =0; ix <n_senrele[iy]; ix++)
			{
				Serial.print("arduino|");
				switch (ix)
				{
					case 0:
						Serial.print("camera_ospiti|");
						break;
					case 1:
						Serial.print("camera_ospiti|");
						break;
					case 2:
						Serial.print("bagno_grande|");
						break;
					case 3:
						Serial.print("studio|");
						break;
					case 4:
						Serial.print("camera_bambini|");
						break;
					case 5:
						Serial.print("camera_giorgia|");
						break;
					case 6:
						Serial.print("sala|");
						break;
				}
				if (ix ==1)
					Serial.print("finestra_1|");
				else
					Serial.print("finestra|");
				if (!f_status[ix])
					Serial.println("CHIUSA");
				else
					Serial.println("APERTA");
			}
		}
		iy++;
		for (ix =0; ix <n_senrele[iy]; ix++)
		{
			s_status[ix] =digitalRead(s_sensori[ix]);
			if (s_stapre[ix] !=s_status[ix] && s_stapre[ix] !=2)
			{
				diverso =1;
				s_stapre[ix] =s_status[ix];
			}
		}
		if (diverso)
		{
			diverso =0;
			for (ix =0; ix <n_senrele[iy]; ix++)
			{
				Serial.print("arduino|");
				switch (ix)
				{
					case 0:
						Serial.print("camera_ospiti|");
						break;
					case 1:
						Serial.print("camera_ospiti|");
						break;
					case 2:
						Serial.print("bagno_grande|");
						break;
					case 3:
						Serial.print("camera_bambini|");
						break;
					case 4:
						Serial.print("camera_giorgia|");
						break;
				}
				if (ix ==1)
					Serial.print("serranda_1|");
				else
					Serial.print("serranda|");
				if (!s_status[ix])
					Serial.println("CHIUSA");
				else
					Serial.println("APERTA");
			}
		}
		iy++;
		for (ix =0; ix <n_senrele[iy]; ix++)
		{
			p_status[ix] =digitalRead(p_sensori[ix]);
			if (p_stapre[ix] !=p_status[ix])
			{
				diverso =1;
				p_stapre[ix] =p_status[ix];
			}
		}
		if (diverso)
		{
			diverso =0;
			for (ix =0; ix <n_senrele[iy]; ix++)
			{
				Serial.print("arduino|");
				switch (ix)
				{
					case 0:
						Serial.print("sala|");
						break;
					case 1:
						Serial.print("corridoio|");
						break;
				}
				Serial.print("porta|");
				if (!p_status[ix])
					Serial.println("CHIUSA");
				else
					Serial.println("APERTA");
			}
		}
		// Serial.print("Fine: ");
		// Serial.println(millis());
	}
  
	for(pulsa =0; pulsa <6; pulsa++)
	{
		int  pulstat =digitalRead(s_pulsanti[pulsa]);
	
		if (!pulstat)
		{
			if (!p_tempo[pulsa])
			{
				p_tempo[pulsa] =millis();
//				Serial.print("ON Pulsante ");
//				Serial.println(s_pulsanti[pulsa]);
			}
		}
		else	if(p_tempo[pulsa])
		{
			unsigned  long  diff =(millis() -p_tempo[pulsa]);
	  
			p_tempo[pulsa] =0;
			
			if (diff >P_MIN  
			&& (a_tempo[pulsa]	|| c_tempo[pulsa]))		//pulsante premuto x interrompere apertura/chisura
			{
				if (pulsa ==5)	// tutti i rele
				{
					for (ix =0; ix <5; ix++)
					{
						if (a_tempo[ix])
						{
							a_tempo[ix] =0L;
							digitalWrite(a_rele[ix], (!a_roff[ix] ? LOW : HIGH));
							s_stapre[ix] =3;
						}
						if (c_tempo[ix])
						{
							c_tempo[ix] =0L;
							digitalWrite(c_rele[ix], (!c_roff[ix] ? LOW : HIGH));
							s_stapre[ix] =3;
						}
					}
					a_tempo[ix] =c_tempo[ix] =0L;
					s_stapre[ix] =3;
				}
				else
				{
					if (a_tempo[pulsa])
					{
						a_tempo[pulsa] =0L;
						digitalWrite(a_rele[pulsa], (!a_roff[pulsa] ? LOW : HIGH));
						s_stapre[pulsa] =3;
					}
					if (c_tempo[pulsa])
					{
						c_tempo[pulsa] =0L;
						digitalWrite(c_rele[pulsa], (!c_roff[pulsa] ? LOW : HIGH));
						s_stapre[pulsa] =3;
					}	
				}
			}
			else	if (diff >P_MIN  && diff <=A_MAX)	//pulsante premuto x chiudere
			{
				if (pulsa ==5)	// tutti i rele
				{
					for (ix =0; ix <5; ix++)
					{
						c_tempo[ix] =millis();
						digitalWrite(a_rele[ix], (!a_roff[ix] ? LOW : HIGH));
						digitalWrite(c_rele[ix], (!c_roff[ix] ? HIGH : LOW));
					}
					c_tempo[ix] =millis();
				}
				else
				{
					c_tempo[pulsa] =millis();
					digitalWrite(c_rele[pulsa], (!c_roff[pulsa] ? HIGH : LOW));
				}
			}
			else	if (diff >P_MIN  && diff >A_MAX)	//pulsante premuto x aprire
			{
				if (pulsa ==5)	// tutti i rele
				{
					for (ix =0; ix <5; ix++)
					{
						a_tempo[ix] =millis();
						digitalWrite(c_rele[ix], (!c_roff[ix] ? LOW : HIGH));
						digitalWrite(a_rele[ix], (!a_roff[ix] ? HIGH : LOW));
					}
					a_tempo[ix] =millis();
				}
				else
				{
					a_tempo[pulsa] =millis();
					digitalWrite(a_rele[pulsa], (!a_roff[pulsa] ? HIGH : LOW));
				}
			}
		}
	}
	for (ix =0 ; ix <6; ix++)
	{
        if ((a_tempo[ix] || c_tempo[ix]) && ix !=5 && s_stapre[ix] !=2)
        {
			Serial.print("arduino|");
            switch (ix)
		    {
				case 0:
					Serial.print("camera_ospiti|");
					break;
				case 1:
					Serial.print("camera_ospiti|");
					break;
				case 2:
					Serial.print("bagno_grande|");
					break;
				case 3:
					Serial.print("camera_bambini|");
					break;
				case 4:
					Serial.print("camera_giorgia|");
					break;
			}

			if (ix ==1)
				Serial.print("serranda_1|");
			else
				Serial.print("serranda|");
			
			s_stapre[ix] =2;
			
			if (a_tempo[ix])
				Serial.println("IN APERTURA");
			else
				Serial.println("IN CHIUSURA");
		}
		if (ix ==5 && (a_tempo[ix] || c_tempo[ix]))
			s_stapre[ix] =2;
		if (a_tempo[ix]
		&& ((millis() -a_tempo[ix] >=T_MAX)
			|| (a_tempo[ix] >millis() && millis() >=T_MAX)))
		{
			a_tempo[ix] =0L;
			if	(ix !=5)
				digitalWrite(a_rele[ix], (!a_roff[ix] ? LOW : HIGH));
			s_stapre[ix] =3;
		}
		if (c_tempo[ix]
		&& ((millis() -c_tempo[ix] >=T_MAX)
				|| (c_tempo[ix] >millis() && millis() >=T_MAX)))
		{
			c_tempo[ix] =0L;
			if	(ix !=5)
				digitalWrite(c_rele[ix], (!c_roff[ix] ? LOW : HIGH));
			s_stapre[ix] =3;
		}
		if (ix !=5
		&& c_tempo[ix]
		&& (millis() -c_tempo[ix]) >=3000	// serranda in chiusura con sensore
		&& !digitalRead(s_sensori[ix]))		// gia' chiuso continuo x 2 secondo
		{
//			Serial.print("Chiuso: ");
//			Serial.print(ix);
//			Serial.print(" tempo: ");
//			Serial.print(millis());
//			Serial.print(" inizio: ");
//			Serial.print(c_tempo[ix]);
			c_tempo[ix] =(millis() -T_MAX -2000);
//			Serial.print(" dopo: ");
//			Serial.println(c_tempo[ix]);
		}					
	}
	for (ix =0 ; ix <n_senrele[5]; ix++)
	{
		if (r_stapre[ix] !=r_status[ix])
		{
			if (!r_status[ix])
				digitalWrite(r_rele[ix], (!r_roff[ix] ? LOW : HIGH));
			else
				digitalWrite(r_rele[ix], (!r_roff[ix] ? HIGH : LOW));
			r_stapre[ix] =r_status[ix];
			Serial.print("arduino|");
			Serial.print("riscaldamento|");
			if (!ix)
				Serial.print("r_zona_giorno|");
			else
				Serial.print("r_zona_notte|");
			if (r_status[ix])
				Serial.println("ACCESO");
			else
				Serial.println("SPENTO");
		}
	}
	if (a_status != a_stapre)
	{
		int	ix;
		int	iy;
		
		a_stapre =a_status;
		Serial.print("arduino|");
		Serial.print("allarme|");
		Serial.print("status|");
		if (a_status)
			Serial.println("ACCESO");
		else
			Serial.println("SPENTO");
		
		for (ix =iy =0; ix <n_senrele[iy]; ix++)
		{
			f_staall[ix] =f_status[ix];
			Serial.print("arduino|");
			switch (ix)
			{
				case 0:
					Serial.print("camera_ospiti|");
					break;
				case 1:
					Serial.print("camera_ospiti|");
					break;
				case 2:
					Serial.print("bagno_grande|");
					break;
				case 3:
					Serial.print("studio|");
					break;
				case 4:
					Serial.print("camera_bambini|");
					break;
				case 5:
					Serial.print("camera_giorgia|");
					break;
				case 6:
					Serial.print("sala|");
					break;
			}
			if (ix ==1)
				Serial.print("a_finestra_1|");
			else
				Serial.print("a_finestra|");
			if (a_status)
			{
				if (!f_status[ix])
					Serial.println("CHIUSA");
				else
					Serial.println("APERTA");
			}
			else
				Serial.println("------");
		}
		iy++;
		for (ix =0; ix <n_senrele[iy]; ix++)
		{
			s_staall[ix] =s_status[ix];
			Serial.print("arduino|");
			switch (ix)
			{
				case 0:
				Serial.print("camera_ospiti|");
					break;
				case 1:
					Serial.print("camera_ospiti|");
					break;
				case 2:
					Serial.print("bagno_grande|");
					break;
				case 3:
					Serial.print("camera_bambini|");
					break;
				case 4:
					Serial.print("camera_giorgia|");
					break;
			}
			if (ix ==1)
				Serial.print("a_serranda_1|");
			else
				Serial.print("a_serranda|");
			if (a_status)
			{
				if (!s_status[ix])
					Serial.println("CHIUSA");
				else
					Serial.println("APERTA");
			}
			else
				Serial.println("------");
		}
		iy++;
		for (ix =0; ix <n_senrele[iy]; ix++)
		{
			p_staall[ix] =p_status[ix];
			Serial.print("arduino|");
			switch (ix)
			{
				case 0:
					Serial.print("sala|");
					break;
				case 1:
					Serial.print("corridoio|");
					break;
			}
			Serial.print("porta|");
			if (a_status)
			{
				if (!p_status[ix])
					Serial.println("CHIUSA");
				else
					Serial.println("APERTA");
			}
			else
				Serial.println("------");
		}
	}
	else
		if (a_status)
	{
		int	ix;
		int	iy;
		
		
		for (ix =iy =0; ix <n_senrele[iy]; ix++)
		{
			if(!f_staall[ix] || f_staall[ix] ==f_status[ix])
				continue;
			f_staall[ix] =f_status[ix];
			Serial.print("arduino|");
			switch (ix)
			{
				case 0:
					Serial.print("camera_ospiti|");
					break;
				case 1:
					Serial.print("camera_ospiti|");
					break;
				case 2:
					Serial.print("bagno_grande|");
					break;
				case 3:
					Serial.print("studio|");
					break;
				case 4:
					Serial.print("camera_bambini|");
					break;
				case 5:
					Serial.print("camera_giorgia|");
					break;
				case 6:
					Serial.print("sala|");
					break;
			}
			if (ix ==1)
				Serial.print("a_finestra_1|");
			else
				Serial.print("a_finestra|");
			if (a_status)
			{
				if (!f_status[ix])
					Serial.println("CHIUSA");
				else
					Serial.println("APERTA");
			}
			else
				Serial.println("------");
		}
		iy++;
		for (ix =0; ix <n_senrele[iy]; ix++)
		{
			if(!s_staall[ix] || s_staall[ix] ==s_status[ix])
				continue;
			s_staall[ix] =s_status[ix];
			Serial.print("arduino|");
			switch (ix)
			{
				case 0:
				Serial.print("camera_ospiti|");
					break;
				case 1:
					Serial.print("camera_ospiti|");
					break;
				case 2:
					Serial.print("bagno_grande|");
					break;
				case 3:
					Serial.print("camera_bambini|");
					break;
				case 4:
					Serial.print("camera_giorgia|");
					break;
			}
			if (ix ==1)
				Serial.print("a_serranda_1|");
			else
				Serial.print("a_serranda|");
			if (a_status)
			{
				if (!s_status[ix])
					Serial.println("CHIUSA");
				else
					Serial.println("APERTA");
			}
			else
				Serial.println("------");
		}
		iy++;
		for (ix =0; ix <n_senrele[iy]; ix++)
		{
			if(!p_staall[ix] || p_staall[ix] ==p_status[ix])
				continue;
			p_staall[ix] =p_status[ix];
			Serial.print("arduino|");
			switch (ix)
			{
				case 0:
					Serial.print("sala|");
					break;
				case 1:
					Serial.print("corridoio|");
					break;
			}
			Serial.print("porta|");
			if (a_status)
			{
				if (!p_status[ix])
					Serial.println("CHIUSA");
				else
					Serial.println("APERTA");
			}
			else
				Serial.println("------");
		}
	}
	if (te_status != te_stapre)
	{
		te_stapre =te_status;
		Serial.print("arduino|");
		Serial.print("telecamera|");
		Serial.print("status|");
		if (a_status)
			Serial.println("ACCESA");
		else
			Serial.println("SPENTA");	
	}
	if (p1_status != p1_stapre)
	{
		p1_stapre =p1_status;
		Serial.print("arduino|");
		Serial.print("presa_1|");
		Serial.print("status|");
		if (a_status)
			Serial.println("ACCESA");
		else
			Serial.println("SPENTA");	
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

