// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();


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
byte  r_automa	[] ={  0, 0};						//status riscaldamento 0 no 1 automatico
byte  r_autpre	[] ={  0, 0};						//status risc. preced. 0 no 1 automatico
byte  a_status	=0;									//status allarme    0 spento 1 acceso 2 in allarme
byte  te_status	=0;									//status telecamera 0 spenta 1 accesa
byte  p1_status	=0;									//status presa 1    0 spenta 1 accesa
byte  f_stapre	[] ={  -1, 0, 0, 0, 0, 0, 0};		//status sensori 0 chiuso 1 aperto
byte  s_stapre	[] ={  -1, 0, 0, 0, 0, 0};
byte  p_stapre	[] ={  -1, 0};
byte  r_stapre	[] ={  0, 0};						//status riscaldamento 0 spento 1 acceso
byte  a_stapre	=0;
byte  te_stapre	=0;									//status telecamera 0 spenta 1 accesa
byte  p1_stapre	=0;									//status presa 1    0 spenta 1 accesa
byte  f_staall	[] ={  -1, 0, 0, 0, 0, 0, 0};		//status finestre serrande e porte
byte  s_staall	[] ={  -1, 0, 0, 0, 0, 0};			//al momento dell'accensione dell'allarme
byte  p_staall	[] ={  -1, 0};
byte  a_pronto	=0;
byte  a_staini	=0;									//stato allarme all'accensione della'arduino
unsigned  long  a_ultlet;							//tempo ultima lettura allarme pronto
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

float  t_ult   [] ={  0, 0, 0, 0, 0, 0};    //temperatura ultima
float  c_ult   [] ={  0, 0, 0, 0, 0, 0};    //correzione alla temeratura letta

float  u_ult   [] ={  0, 0, 0, 0, 0, 0};    //umidita ultima

int	  s_ult =0;	// numero sensore da leggere

String	inputString = "";         // a string to hold incoming data
boolean	stringComplete = false;  // whether the string is complete
String	inputString1 = "";         // a string to hold incoming data
boolean	stringComplete1 = false;  // whether the string is complete

char	riga[20];
char	colonna[20];
char	risposta[20];
int	accensione =1;					//elementi -1 da controllare prima del decorso normale

int	elementi_allarme =1;			//elementi -1 da controllare per allarme
int	elementi_esclusi =0;			//elementi -1 controllo esclusione
byte  f_allesc	[] ={  0, 0, 0, 0, 0, 0, 0};		//status sensori 0 inluso 1 escluso
byte  s_allesc	[] ={  0, 0, 0, 0, 0, 0};
byte  p_allesc	[] ={  0, 0};
char  messaggio_allarme[100];

unsigned  long  te_ultlet;			// tempo di spegnimento allarme in allarme dopo accensione obbligata
int   te_accall =0;					// telecamera accesa da allarme
unsigned  long  p1_ultlet;			// tempo di spegnimento lampada dopo accensione obbligata
int   p1_accall =0;					// presa_1 accesa da allarme

float	min_temp	[] ={	-100,	-100};	// temperatura  minima  zone modalita' automatica
float	max_temp	[] ={	-100,	-100};	// temperatura  massima zone modalita' automatica
float	umi_temp	[] ={	-100,	-100};	// temp. ultima minima  zone modalita' automatica
float	uma_temp	[] ={	-100,	-100};	// temp. ultima massima zone modalita' automatica
float	con_temp	[] ={	-100,	-100};	// temperatura  attuale zone confronto x automatica
											// riferimento  sala zona giorno bambini x notte
float	con_tepr	[] ={	-100,	-100};	// temperatura  preced. zone confronto x automatica
											// riferimento  sala zona giorno bambini x notte


void setup() 
{
	int ix ;
	int iy;

	t_ultlet =0L;
	Serial.begin(9600);
	Serial.println("Marietto test!");
	delay(50);
	Serial1.begin(9600);
	 // Transmitter is connected to Arduino Pin #14  
	mySwitch.enableTransmit(14);
	// Optional set pulse length.0
	// Recive demo advanced dice 231 millsecondi
	mySwitch.setPulseLength(230);

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
	inputString1.reserve(200);
}

void loop() 
{
	int  pulsa;
	int  ix;
	int  iy;
	
	if (accensione)
	{
		umi_temp[0] =min_temp[0] =15.0;
		umi_temp[1] =min_temp[1] =16.0;
		uma_temp[0] =max_temp[0] =20.0;
		uma_temp[1] =max_temp[1] =21.0;
		accensione--;
	}
		
	if (accensione)
	{
		switch(accensione)
		{		
			case 14:
				strcpy(riga, "camera_ospiti");
				strcpy(colonna, "err_temp");
				break;
			case 13:
				strcpy(riga, "camera_bambini");
				strcpy(colonna, "err_temp");
				break;
			case 12:
				strcpy(riga, "camera_giorgia");
				strcpy(colonna, "err_temp");
				break;
			case 11:
				strcpy(riga, "sala");
				strcpy(colonna, "err_temp");
				break;
			case 10:
				strcpy(riga, "studio");
				strcpy(colonna, "err_temp");
				break;
			case 9:
				strcpy(riga, "valore_minimo");
				strcpy(colonna, "zona_giorno");
				break;
			case 8:
				strcpy(riga, "valore_minimo");
				strcpy(colonna, "zona_notte");
				break;
			case 7:
				strcpy(riga, "valore_massimo");
				strcpy(colonna, "zona_giorno");
				break;
			case 6:
				strcpy(riga, "valore_massimo");
				strcpy(colonna, "zona_notte");
				break;
			case 5:
				strcpy(riga, "riscaldamento");
				strcpy(colonna, "zona_giorno");
				break;
			case 4:
				strcpy(riga, "riscaldamento");
				strcpy(colonna, "zona_notte");
				break;
			case 3:
				strcpy(riga, "telecamera");
				strcpy(colonna, "status");
				break;
			case 2:
				strcpy(riga, "presa_1");
				strcpy(colonna, "status");
				break;
			case 1:
				strcpy(riga, "allarme");
				strcpy(colonna, "status");
				break;
		}
		stringComplete = false;
		leggi_db();
	}
	
	if (elementi_esclusi)
		elementi_esclusi--;
	
	if (elementi_esclusi)
	{
		switch(elementi_esclusi)
		{
			case 14:
				strcpy(riga, "camera_ospiti");
				strcpy(colonna, "e_finestra");
				break;
			case 13:
				strcpy(riga, "camera_ospiti");
				strcpy(colonna, "e_finestra_1");
				break;
			case 12:
				strcpy(riga, "bagno_grande");
				strcpy(colonna, "e_finestra");
				break;
			case 11:
				strcpy(riga, "studio");
				strcpy(colonna, "e_finestra");
				break;
			case 10:
				strcpy(riga, "camera_bambini");
				strcpy(colonna, "e_finestra");
				break;
			case 9:
				strcpy(riga, "camera_giorgia");
				strcpy(colonna, "e_finestra");
				break;
			case 8:
				strcpy(riga, "sala");
				strcpy(colonna, "e_finestra");
				break;
			case 7:
				strcpy(riga, "camera_ospiti");
				strcpy(colonna, "e_serranda");
				break;
			case 6:
				strcpy(riga, "camera_ospiti");
				strcpy(colonna, "e_serranda_1");
				break;
			case 5:
				strcpy(riga, "bagno_grande");
				strcpy(colonna, "e_serranda");
				break;
			case 4:
				strcpy(riga, "camera_bambini");
				strcpy(colonna, "e_serranda");
				break;
			case 3:
				strcpy(riga, "camera_giorgia");
				strcpy(colonna, "e_serranda");
				break;
			case 2:
				strcpy(riga, "sala");
				strcpy(colonna, "e_porta");
				break;
			case 1:
				strcpy(riga, "corridoio");
				strcpy(colonna, "e_porta");
				break;
		}
		stringComplete = false;
		leggi_db();
	}
	
	// if	(!a_pronto
	// &&	t_ultlet
	// && (millis() -t_ultlet) >=10000L)
	// {
		// Serial.println(" Richiedo Stato a arduino uno --------------------->");
		// Serial1.println("Stato");
		// delay(50);
	// }
	
	serialEvent1(); //call the function
	// print the string when a newline arrives:
	
	if (stringComplete1)
	{
		char	appo[10];
		
		appo[0] =inputString1[0];
		appo[1] =inputString1[1];
		appo[2] ='\0';

		// 00=> Sistema di allarme spento
		// 01=> Sistema di allarme acceso
		// 02==> Allarme in ALLARME

		if (!strcmp(appo, "00"))
		{
			if (!a_pronto && a_staini)
			{
				if (a_staini ==1)
					Serial1.println("Accendi");
				else
				{
					Serial1.println("Aiuto");
					Serial.println("arduino_allarme|IN ALLARME|Richiesta di AIUTO");
					a_status =2;
				}
			}
			else	a_status =0;

			a_pronto =1;
		}
		else	if (!strcmp(appo, "01"))
		{
			a_status =1;
			a_pronto =1;
		}
		else	if (!strcmp(appo, "02"))
		{
			a_status =2;
			if (a_status != a_stapre)
				a_pronto =1;
		}
		inputString1 = "";
		stringComplete1 = false;
	}
	
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
			//	raspberry|valore_minimo|zona_giorno|15.43
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
					else
						if (!strcmp(appo, "valore_minimo"))
						stanza =10;
					else
						if (!strcmp(appo, "valore_massimo"))
						stanza =11;
					
					if (stanza <0)
						break;
				}
				if (iy == 3)
				{
					if ((!strcmp(appo, "serranda") && stanza <6)
					|| (!strcmp(appo, "zona_giorno") && stanza ==6)
					|| stanza >6)
						serranda =0;
					else
						if ((!strcmp(appo, "serranda_1") && stanza <6)
						|| (!strcmp(appo, "zona_notte") && stanza ==6))
						serranda =1;
					else
						if ((!strcmp(appo, "zona_giorno") && stanza ==10)
						|| (!strcmp(appo, "zona_giorno") && stanza ==11))
						serranda =0;
					else
						if ((!strcmp(appo, "zona_notte") && stanza ==10)
						|| (!strcmp(appo, "zona_notte") && stanza ==11))
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
				else
					if (!strcmp(appo, "AIUTO"))
					comando =3;
				else
					if (!strcmp(appo, "AUTOMATICO"))
					comando =4;
				else
					if (stanza ==10)
					{
							min_temp[serranda] =atof(appo);
							comando =5;
					}
				else
					if (stanza ==11)
					{
							max_temp[serranda] =atof(appo);
							comando =6;
					}
				if (comando != -1)
					Serial.println(inputString);
			}
		}
		if (stanza ==-1 && accensione)
		{
			for(ix =iy =iz =0; ix <inputString.length(); ix++, iz++)
			{
				//	raspberry_select|SPENTO
				if (inputString[ix] =='|')
				{
					iy++;
					appo[iz] ='\0';
					if (iy == 1)
					{
						if (strcmp(appo, "raspberry_select"))
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
			
				if (iy == 2)
				{
					if (accensione >5)
					{
						switch(accensione)
						{
							case 14:
								c_ult[0] =atof(appo);
								break;
							case 13:
								c_ult[1] =atof(appo);
								break;
							case 12:
								c_ult[2] =atof(appo);
								break;
							case 11:
								c_ult[3] =atof(appo);
								break;
							case 10:
								c_ult[4] =atof(appo);
								break;
							case 9:
								umi_temp[0] =min_temp[0] =15.0;
								break;
							case 8:
								umi_temp[1] =min_temp[1] =16.0;
								break;
							case 7:
								uma_temp[0] =max_temp[0] =20.0;
								break;
							case 6:
								uma_temp[1] =max_temp[1] =21.0;
								break;
						}								
					}
					else	if (!strncmp(appo, "SPENT", 5))
						comando =-1;
					else
						if (!strncmp(appo, "ACCES", 5))
						{
							switch(accensione)
							{
								case 5:
									r_status[0] =1;
									r_autpre[0] =r_automa[0] =0;
									break;
								case 4:
									r_status[1] =1;
									r_autpre[1] =r_automa[1] =0;
									break;
								case 3:
									te_status =1;;
									break;
								case 2:
									p1_status =1;
									break;
								case 1:
									a_staini =1;
									break;
							}
						}
						if (!strncmp(appo, "A/ACCESO", 8)
						|| !strncmp(appo, "A/SPENTO", 8))
						{
							switch(accensione)
							{
								case 5:
									if (!strncmp(appo, "A/ACCESO", 8))
										r_status[0] =1;
									else
										r_status[0] =0;
									r_autpre[0] =r_automa[0] =1;
									break;
								case 4:
									if (!strncmp(appo, "A/ACCESO", 8))
										r_status[1] =1;
									else
										r_status[1] =0;
									r_autpre[1] =r_automa[1] =1;
									break;
								case 3:
								case 2:
								case 1:
									break;
							}
						}
					else
						if (!strncmp(appo, "IN ALLARME", 10))
						{
							switch(accensione)
							{
								case 1:
									a_staini =2;
									break;
								default:
									break;
							}
						}
					else
						{
							switch(accensione)
							{
								case 5:
									r_stapre[0] =-1;
									break;
								case 4:
									r_stapre[1] =-1;
									break;
								case 3:
									te_stapre =-1;
									break;
								case 2:
									p1_stapre =-1;
									break;
								case 1:
									break;
							}
						}
				}
			}
		}
		if (stanza ==-1 && elementi_esclusi)
		{
			for(ix =iy =iz =0; ix <inputString.length(); ix++, iz++)
			{
				//	raspberry_select|ESCLUSO
				if (inputString[ix] =='|')
				{
					iy++;
					appo[iz] ='\0';
					if (iy == 1)
					{
						if (strcmp(appo, "raspberry_select"))
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
			
				if (iy == 2)
				{
					int	escl =0;

					if (!strncmp(appo, "ESCL", 4))
						escl =1;
					
					switch(elementi_esclusi)
					{
						case 14:
							f_allesc[0] =escl;
							Serial.print("allesc[0] -> ");
							Serial.print(escl);
							Serial.print(" <- ");
							Serial.println(appo);
							break;
						case 13:
							f_allesc[1] =escl;
							Serial.print("allesc[1] -> ");
							Serial.print(escl);
							Serial.print(" <- ");
							Serial.println(appo);
							break;
						case 12:
							f_allesc[2] =escl;
							break;
						case 11:
							f_allesc[3] =escl;
							break;
						case 10:
							f_allesc[4] =escl;
							break;
						case 9:
							f_allesc[5] =escl;
							break;
						case 8:
							f_allesc[6] =escl;
							break;
						case 7:
							s_allesc[0] =escl;
							break;
						case 6:
							s_allesc[1] =escl;
							break;
						case 5:
							s_allesc[2] =escl;
							break;
						case 4:
							s_allesc[3] =escl;
							break;
						case 3:
							s_allesc[4] =escl;
							break;
						case 2:
							p_allesc[0] =escl;
							break;
						case 1:
							p_allesc[1] =escl;
							break;
					}
				}
			}
		}
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
				{
						r_status[serranda] =1;
						r_automa[serranda] =0;
				}
				else	if (stanza ==6 && comando ==1)
				{
						r_status[serranda] =0;
						r_automa[serranda] =0;
				}
				else	if (stanza ==6 && comando ==4)
						r_automa[serranda] =1;
				else	if (stanza ==7 && !comando)
						Serial1.println("Accendi");
				else	if (stanza ==7 && comando ==1)
						Serial1.println("Spegni");
				else	if (stanza ==7 && comando ==3)
				{
						Serial1.println("Aiuto");
						Serial.println("arduino_allarme|IN ALLARME|Richiesta di AIUTO");
						a_status =2;
				}
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
		
	if ((!accensione && !elementi_esclusi)
	&& (!t_ultlet
	|| ((millis() -t_ultlet) >=10000L)))
	{
		int	diverso =0;
		float h;
		float t;
		float hic;
		float t_corr;
		
		t_ultlet =millis();
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
			Serial.print("arduino|");
			
			switch (s_ult)
			{
				case 0:
					Serial.print("camera_ospiti|");
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
					Serial.print("camera_ospiti|");
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
				
		if (t !=t_ult[s_ult]
		&& (!isnan(t) || !isnan(t_ult[s_ult])))
		{
			Serial.print("arduino|");
			
			switch (s_ult)
			{
				case 0:
					Serial.print("camera_ospiti|");
					t_corr =t +c_ult[s_ult];
					break;
				case 1:
					Serial.print("camera_bambini|");
					t_corr =t +c_ult[s_ult];
					con_temp[1] =15.0;
					break;
				case 2:
					Serial.print("camera_giorgia|");
					t_corr =t +c_ult[s_ult];
					break;
				case 3:
					Serial.print("sala|");
					t_corr =t +c_ult[s_ult];
					con_temp[0] =16.0;
					break;
				case 4:
					Serial.print("studio|");
					t_corr =t +c_ult[s_ult];
					break;
			}
			Serial.print("temperatura_percepita|");
			Serial.println(t_corr);
		}
		// if ((t !=t_ult[s_ult] || h !=u_ult[s_ult])
		// && (!isnan(t) || !isnan(t_ult[s_ult]) || !isnan(h) || !isnan(u_ult[s_ult])))
		// {
			// Serial.print("arduino|");
			
			// switch (s_ult)
			// {
				// case 0:
					// Serial.print("camera_ospiti|");
					// hic = t_camosp.computeHeatIndex(t_corr, h, false);
					// break;
				// case 1:
					// Serial.print("camera_bambini|");
					// hic = t_cambam.computeHeatIndex(t_corr, h, false);
					// if (!isnan(hic))
						// con_temp[1] =hic;
					// else
						// con_temp[1] =15.00;
					// break;
				// case 2:
					// Serial.print("camera_giorgia|");
					// hic = t_camgio.computeHeatIndex(t_corr, h, false);
					// break;
				// case 3:
					// Serial.print("sala|");
					// hic = t_salone.computeHeatIndex(t_corr, h, false);
					// if (!isnan(hic))
						// con_temp[0] =hic;
					// else
						// con_temp[0] =15.00;
					// break;
				// case 4:
					// Serial.print("studio|");
					// hic = t_studio.computeHeatIndex(t_corr, h, false);
					// break;
			// }
			// Serial.print("temperatura_percepita|");
			// Serial.println(hic);
		// }

		u_ult[s_ult] =h;
		t_ult[s_ult] =t;

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
	 
	for(pulsa =0; pulsa <6 && !accensione && !elementi_esclusi; pulsa++)
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
	for (ix =0 ; ix <6 && !accensione && !elementi_esclusi; ix++)
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
		if (r_automa[ix] ==1
		&& (con_temp[ix] != con_tepr[ix]
			|| min_temp[ix] !=umi_temp[ix]
			|| max_temp[ix] !=uma_temp[ix]
			|| r_automa[ix] !=r_autpre[ix]))
		{
			Serial.print("arduino|");
			Serial.print("riscaldamento|");
			if (!ix)
				Serial.print("zona_giorno|");
			else
				Serial.print("zona_notte|");
			
			if (max_temp[ix] <min_temp[ix])
				max_temp[ix] =min_temp[ix];
			
			if (con_temp[ix] <min_temp[ix])
			{
				digitalWrite(r_rele[ix], (!r_roff[ix] ? HIGH : LOW));
				Serial.println("A/ACCESO");
				r_status[ix] =1;
			}
			else	if (con_temp[ix] >max_temp[ix])
			{
				digitalWrite(r_rele[ix], (!r_roff[ix] ? LOW : HIGH));
				Serial.println("A/SPENTO");
				r_status[ix] =0;
			}
			else	if (r_status[ix])
				Serial.println("A/ACCESO");
			else	if (!r_status[ix])
				Serial.println("A/SPENTO");
			else	Serial.println("A/??????");
			con_tepr[ix] =con_temp[ix];
			r_stapre[ix] =r_status[ix];
			r_autpre[ix] =r_automa[ix];
			if (umi_temp[ix] !=min_temp[ix])
			{
				Serial.print("arduino|");
				Serial.print("valore_minimo|");
				if (!ix)
					Serial.print("zona_giorno|");
				else
					Serial.print("zona_notte|");
				umi_temp[ix] =min_temp[ix];
				Serial.println(umi_temp[ix]);				
			}
			if (uma_temp[ix] !=max_temp[ix])
			{
				Serial.print("arduino|");
				Serial.print("valore_massimo|");
				if (!ix)
					Serial.print("zona_giorno|");
				else
					Serial.print("zona_notte|");
				uma_temp[ix] =max_temp[ix];
				Serial.println(uma_temp[ix]);				
			}
		}
		if ((!r_automa[ix] && r_autpre[ix] !=r_automa[ix])
		|| (r_stapre[ix] !=r_status[ix] && r_status[ix] !=2))
		{
			if (!r_status[ix])
				digitalWrite(r_rele[ix], (!r_roff[ix] ? LOW : HIGH));
			else
				digitalWrite(r_rele[ix], (!r_roff[ix] ? HIGH : LOW));
			r_stapre[ix] =r_status[ix];
			
			Serial.print("arduino|");
			Serial.print("riscaldamento|");
			if (!ix)
				Serial.print("zona_giorno|");
			else
				Serial.print("zona_notte|");
			if (r_status[ix])
				Serial.println("ACCESO");
			else
				Serial.println("SPENTO");
			r_autpre[ix] =r_automa[ix];
		}
	}
	if (a_status != a_stapre)
	{
		int	ix;
		int	iy;

		// salvataggio situazione all'accensione dell'allarme
		// o eventuale cancellazione nel caso di spegnimento
		if (a_status ==1)
			elementi_esclusi =elementi_allarme;
		
		a_stapre =a_status;
		Serial.print("arduino|allarme|status|");
		if (a_status ==1)
			Serial.println("ACCESO");
		else	if (a_status ==2)
			Serial.println("IN ALLARME");
		else
			Serial.println("SPENTO");
		// Aggiorno anche il log_allarme
		if (a_status !=2)
		{
			Serial.print("arduino_allarme|");
			if (a_status ==1)
				Serial.println("ACCESO|------");
			else
				Serial.println("SPENTO|------");
		}
		
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
			Serial.print("a_porta|");
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
	else	if (a_status)
	{
		int	ix;
		int	iy;
		
		// nel caso di finestra serranda o porta aperta nel momento dell'accensione
		// se la stessa si richiude torna ad essere controllata
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
			Serial.print("a_porta|");
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
		// controllo se attivare l'allarme o meno
		// cercando di tener conto degli elementi esclusi
		for (ix =iy =0; ix <n_senrele[iy]; ix++)
		{
			if(f_staall[ix] || f_allesc[ix] || f_staall[ix] ==f_status[ix])
				continue;
			f_staall[ix] =f_status[ix];
			strcpy(messaggio_allarme, "Allarme,");
			switch (ix)
			{
				case 0:
				case 1:
					sprintf(&messaggio_allarme[(strlen(messaggio_allarme))], "%s", "camera_ospiti-");
					break;
				case 2:
					sprintf(&messaggio_allarme[(strlen(messaggio_allarme))], "%s", "bagno_grande-");
					break;
				case 3:
					sprintf(&messaggio_allarme[(strlen(messaggio_allarme))], "%s", "studio-");
					break;
				case 4:
					sprintf(&messaggio_allarme[(strlen(messaggio_allarme))], "%s", "camera_bambini-");
					break;
				case 5:
					sprintf(&messaggio_allarme[(strlen(messaggio_allarme))], "%s", "camera_giorgia-");
					break;
				case 6:
					sprintf(&messaggio_allarme[(strlen(messaggio_allarme))], "%s", "sala-");
					break;
			}
			if (ix ==1)
				sprintf(&messaggio_allarme[(strlen(messaggio_allarme))], "%s", "finestra lato ingresso");
			else
				sprintf(&messaggio_allarme[(strlen(messaggio_allarme))], "%s", "finestra");
			Serial.println("arduino_allarme|IN ALLARME|" + String(messaggio_allarme));
			Serial1.println(messaggio_allarme);
			a_status =2;
		}
		iy++;
		for (ix =0; ix <n_senrele[iy]; ix++)
		{
			if(s_staall[ix] || s_allesc[ix] || s_staall[ix] ==s_status[ix])
				continue;
			s_staall[ix] =s_status[ix];
			strcpy(messaggio_allarme, "Allarme,");
			
			switch (ix)
			{
				case 0:
				case 1:
					sprintf(&messaggio_allarme[(strlen(messaggio_allarme))], "%s", "camera_ospiti-");
					break;
				case 2:
					sprintf(&messaggio_allarme[(strlen(messaggio_allarme))], "%s", "bagno_grande-");
					break;
				case 3:
					sprintf(&messaggio_allarme[(strlen(messaggio_allarme))], "%s", "camera_bambini-");
					break;
				case 4:
					sprintf(&messaggio_allarme[(strlen(messaggio_allarme))], "%s", "camera_giorgia-");
					break;
			}
			if (ix ==1)
				sprintf(&messaggio_allarme[(strlen(messaggio_allarme))], "%s", "serranda lato ingresso");
			else
				sprintf(&messaggio_allarme[(strlen(messaggio_allarme))], "%s", "serranda");
			Serial.println("arduino_allarme|IN ALLARME|" + String(messaggio_allarme));
			Serial1.println(messaggio_allarme);
			a_status =2;
		}
		iy++;
		for (ix =0; ix <n_senrele[iy]; ix++)
		{
			if(p_staall[ix] || p_allesc[ix] || p_staall[ix] ==p_status[ix])
				continue;
			p_staall[ix] =p_status[ix];
			strcpy(messaggio_allarme, "Allarme,");
			switch (ix)
			{
				case 0:
					sprintf(&messaggio_allarme[(strlen(messaggio_allarme))], "%s", "sala-");
					break;
				case 1:
					sprintf(&messaggio_allarme[(strlen(messaggio_allarme))], "%s", "corridoio-");
					break;
			}
			sprintf(&messaggio_allarme[(strlen(messaggio_allarme))], "%s", "porta");
			Serial.println("arduino_allarme|IN ALLARME|" + String(messaggio_allarme));
			Serial1.println(messaggio_allarme);
			a_status =2;
		}
	}

	if (!te_status && a_status ==2)
	{
		Serial.print("Predispongo accensione TELECAMERA");
		te_accall =1;
		te_status =1;
		te_ultlet =0L;
	}
	if (a_status !=2 && te_accall)
	{
		if (!te_ultlet)
			te_ultlet =millis();
		else
		{
			if ((millis() -te_ultlet) >=300000L)
				p1_status =te_status =te_accall =0;
		}
	}
	if (te_status != te_stapre)
	{
		te_stapre =te_status;
		Serial.print("arduino|");
		Serial.print("telecamera|");
		Serial.print("status|");
		if (te_status)
		{
			Serial.println("ACCESA");
			mySwitch.send(4128960, 24);  // presa 1 ON
		}
		else
		{
			Serial.println("SPENTA");
			mySwitch.send(4128768, 24);  // presa 1 OFF
		}		
	}
	if (a_status)
	{
		p1_accall =1;
		if (!p1_ultlet)
			p1_ultlet =millis();
		if (a_status ==1 && ((millis() -p1_ultlet) >=10000L))
		{
			p1_status =1;
			p1_ultlet =millis();
		}
		if (a_status ==2)
			p1_status =1;		
	}
	if (!a_status && p1_accall)
		p1_accall =p1_ultlet =0;
	if (p1_status != p1_stapre)
	{
		p1_stapre =p1_status;
		if (!p1_accall)
		{
			Serial.print("arduino|");
			Serial.print("presa_1|");
			Serial.print("status|");
		}
		if (p1_status)
		{
			if (!p1_accall)
				Serial.println("ACCESA");
			mySwitch.send(983232, 24);  //  presa 2 ON
		}
		else
		{
			Serial.println("SPENTA");
			mySwitch.send(983040, 24);  //  presa 2 OFF
		}
		if (p1_accall)
		{
			delay(20);
			mySwitch.send(983040, 24);  //  presa 2 OFF
			p1_status =p1_stapre =0;
			if (a_status ==2)
			{
				delay(1000);
				mySwitch.send(983232, 24);  //  presa 2 ON
				p1_status =1;
			}
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

void serialEvent1()
{
	while (Serial1.available())
	{
		char inChar = (char)Serial1.read();
		if (inChar == '\n' || inChar == '\r')
		{
			inputString1 += "";
			stringComplete1 = true;
			break;
		}
		else	inputString1 += inChar;	// add it to the inputString:
	}
	if (stringComplete1 ==true && inputString1.length() <4)
	{
		inputString1 = "";
		stringComplete1 = false;
	}
	else
		if (stringComplete1 ==true)
		{
			delay(100);
			Serial.println("Da arduino uno -> " +String(inputString1));
			Serial1.flush();
			delay(100);
		}
}

void leggi_db()
{
	Serial.println("arduino_select|" +String(riga) +String('|') +String(colonna));
	delay(100);
	while(stringComplete ==false)
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
			if (stringComplete ==true && inputString.length() <4)
			{
				inputString = "";
				stringComplete = false;
			}
			else
			if (stringComplete ==true)
			{
				Serial.print("Da select 	-> ");
				Serial.print(inputString);
				Serial.print(" - ");
				Serial.print("accensione : " +String(accensione));
				Serial.println(" elementi_esclusi : " +String(elementi_esclusi));
				break;
			}
		}
	}
}		

