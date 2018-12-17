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


byte  n_senrele[] ={  7,  //numero sensori finestre
					  5,  //numero sensori serrande
					  2,  //numero sensoro portoni
					  5,  //numero rele apertura
					  5}; //numero rele chiusura
byte  f_sensori	[] ={  20, 22, 24, 26, 28, 30, 32};	//sensori finestre
byte  s_sensori	[] ={  21, 23, 25, 29, 31};			//sensori serrande
byte  p_sensori	[] ={  33, 27};						//sensori portoni
byte  a_rele	[] ={  40, 42, 44, 46, 48};			//rele    apertura
byte  a_roff	[] ={  1,  1,  1,  1,  1};			//Status off rele
byte  c_rele	[] ={  41, 43, 45, 47, 49};			//rele    chiusura
byte  c_roff	[] ={  1,  1,  1,  1,  1};			//Status off rele
byte  f_status	[] ={  0, 0, 0, 0, 0, 0, 0};		//status sensori 0 chiuso 1 aperto
byte  s_status	[] ={  0, 0, 0, 0, 0};
byte  p_status	[] ={  0, 0};
byte  f_stapre	[] ={  -1, 0, 0, 0, 0, 0, 0};		//status sensori 0 chiuso 1 aperto
byte  s_stapre	[] ={  -1, 0, 0, 0, 0};
byte  p_stapre	[] ={  -1, 0};
unsigned  long  a_tempo   [] ={  0, 0, 0, 0, 0, 0};    //rele     apertura tempo iniziale apertura
unsigned  long  c_tempo   [] ={  0, 0, 0, 0, 0, 0};    //rele     chiusura tempo iniziale chisura
unsigned  long  p_tempo   [] ={  0, 0, 0, 0, 0, 0};    //pulsante chiusura tempo iniziale chisura
unsigned  long  t_ultlet;                           // tempo ultima lettura
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
		Serial.println(inputString);
		// clear the string:
		inputString = "";
		stringComplete = false;
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
				Serial.print("arduino|");
				Serial.print("camera_ospiti|");
				h = t_camosp.readHumidity();
				Serial.print("umidita|");
				Serial.println(h);
				// Read temperature as Celsius (the default)
				t = t_camosp.readTemperature();
				Serial.print("arduino|");
				Serial.print("camera_ospiti|");
				Serial.print("temperatura|");
				Serial.println(t);
				hic = t_camosp.computeHeatIndex(t, h, false);
				Serial.print("arduino|");
				Serial.print("camera_ospiti|");
				Serial.print("temperatura_percepita|");
				Serial.println(hic);
				break;
			case 1:
				Serial.print("arduino|");
				Serial.print("camera_bambini|");
				h = t_cambam.readHumidity();
				Serial.print("umidita|");
				Serial.println(h);
				// Read temperature as Celsius (the default)
				t = t_cambam.readTemperature();
				Serial.print("arduino|");
				Serial.print("camera_bambini|");
				Serial.print("temperatura|");
				Serial.println(t);
				hic = t_cambam.computeHeatIndex(t, h, false);
				Serial.print("arduino|");
				Serial.print("camera_bambini|");
				Serial.print("temperatura_percepita|");
				Serial.println(hic);
				break;
			case 2:
				Serial.print("arduino|");
				Serial.print("camera_giorgia|");
				h = t_camgio.readHumidity();
				Serial.print("umidita|");
				Serial.println(h);
				// Read temperature as Celsius (the default)
				t = t_camgio.readTemperature();
				Serial.print("arduino|");
				Serial.print("camera_giorgia|");
				Serial.print("temperatura|");
				Serial.println(t);
				hic = t_camgio.computeHeatIndex(t, h, false);
				Serial.print("arduino|");
				Serial.print("camera_giorgia|");
				Serial.print("temperatura_percepita|");
				Serial.println(hic);
				break;
			case 3:
				Serial.print("arduino|");
				Serial.print("sala|");
				h = t_salone.readHumidity();
				Serial.print("umidita|");
				Serial.println(h);
				// Read temperature as Celsius (the default)
				t = t_salone.readTemperature();
				Serial.print("arduino|");
				Serial.print("sala|");
				Serial.print("temperatura|");
				Serial.println(t);
				hic = t_salone.computeHeatIndex(t, h, false);
				Serial.print("arduino|");
				Serial.print("sala|");
				Serial.print("temperatura_percepita|");
				Serial.println(hic);
				break;
			case 4:
				Serial.print("arduino|");
				Serial.print("studio|");
				h = t_studio.readHumidity();
				Serial.print("umidita|");
				Serial.println(h);
				// Read temperature as Celsius (the default)
				t = t_studio.readTemperature();
				Serial.print("arduino|");
				Serial.print("studio|");
				Serial.print("temperatura|");
				Serial.println(t);
				hic = t_studio.computeHeatIndex(t, h, false);
				Serial.print("arduino|");
				Serial.print("studio|");
				Serial.print("temperatura_percepita|");
				Serial.println(hic);
				break;
			case 5:
				Serial.print("arduino|");
				Serial.print("bagno_piccolo|");
				h = t_bagnop.readHumidity();
				Serial.print("umidita|");
				Serial.println(h);
				// Read temperature as Celsius (the default)
				t = t_bagnop.readTemperature();
				Serial.print("arduino|");
				Serial.print("bagno_piccolo|");
				Serial.print("temperatura|");
				Serial.println(t);
				hic = t_bagnop.computeHeatIndex(t, h, false);
				Serial.print("arduino|");
				Serial.print("bagno_piccolo|");
				Serial.print("temperatura_percepita|");
				Serial.println(hic);
				break;
			
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
			if (s_stapre[ix] !=s_status[ix])
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
							s_stapre[ix] =2;
						}
						if (c_tempo[ix])
						{
							c_tempo[ix] =0L;
							digitalWrite(c_rele[ix], (!c_roff[ix] ? LOW : HIGH));
							s_stapre[ix] =2;
						}
					}
					a_tempo[ix] =c_tempo[ix] =0L;
				}
				else
				{
					if (a_tempo[pulsa])
					{
						a_tempo[pulsa] =0L;
						digitalWrite(a_rele[pulsa], (!a_roff[pulsa] ? LOW : HIGH));
						s_stapre[pulsa] =2;
					}
					if (c_tempo[pulsa])
					{
						c_tempo[pulsa] =0L;
						digitalWrite(c_rele[pulsa], (!c_roff[pulsa] ? LOW : HIGH));
						s_stapre[pulsa] =2;
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
        if ((a_tempo[ix] || c_tempo[ix]) && ix !=5)
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
			if (a_tempo[ix])
			{
				s_stapre[ix] =2;
				Serial.println("IN APERTURA");
			}
			else
				Serial.println("IN CHIUSURA");
		}
		if (a_tempo[ix]
		&& ((millis() -a_tempo[ix] >=T_MAX)
			|| (a_tempo[ix] >millis() && millis() >=T_MAX)))
		{
			a_tempo[ix] =0L;
			if	(ix !=5)
				digitalWrite(a_rele[ix], (!a_roff[ix] ? LOW : HIGH));
		}
		if (c_tempo[ix]
		&& ((millis() -c_tempo[ix] >=T_MAX)
				|| (c_tempo[ix] >millis() && millis() >=T_MAX)))
		{
			c_tempo[ix] =0L;
			if	(ix !=5)
				digitalWrite(c_rele[ix], (!c_roff[ix] ? LOW : HIGH));
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

