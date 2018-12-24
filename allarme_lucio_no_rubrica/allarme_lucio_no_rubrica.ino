
/* -----------------------------------------------------------------------------------
 Scopo del programma è simulare la gestione di un Allarme (simulato con la pressione
 di un PUSHBUTTON). L'allarme puo' essere disinserito con un opportuno SMS inviato
 da un cellulare autorizzato
 Questa dovrebbe eesere l'ultima versione di GIORGIA
----------------------------------------------------------------------------------- */

#include "SIM900.h"
#include <SoftwareSerial.h>   // Necessaria alla libreria gsm.h richiamata in sms.h
#include "sms.h"              // Libreria per la gestione degli SMS
SMSGSM sms;

#include <RCSwitch.h>
RCSwitch mySwitch = RCSwitch();
   
#define LED 13      // LED collegato al pin digitale 13
#define SIRENA 12	// PIN collegato alla sirena 
 
int		val = 0;    // val conserva lo stato attuale del pin di input (7) 
int		prec_val;   // Variabile per valutare lo stato nel precedente Loop
boolean	acceso;     // Acceso = true  => il LED è acceso
boolean	GSMstarted=false;
boolean	SMSInviato;
char	smsbuffer[160];
char	messaggio[60];
char	Mittente[20];
int		allarme =0;
int		primo_giro =0;
int		arduino =0;
int		led_status =0;
int		indice_rubrica =0;  //  indice inizio rubrica allarme

int		i_acceso	=0;
int		i_spento	=0;
int		i_aiuto	=0;
int		i_notte	=0;
int		i_tipo_1	=0;
int		i_test	=0;

int		i_messaggi	=0;
int		i_telefono	=0;
int		i_sirena	=0;

char	w_azione	[][11] ={  "ACCESO", "SPENTO", "NOTTE", "TIPO_1", "TEST", "IN_ALLARME", "AIUTO"};
int		w_cicli	[] ={  1, 1, 1, 1, 1, 1, 1};
int		w_tempo_ciclo	[] ={ 10, 10, 10, 10, 10, 10, 10};
int		w_tempo_intervallo	[] ={ 500, 500, 500, 500, 500, 500, 500};

char	r_nome		[10][51];
char	r_numero	[10][21];
int		r_telefona	[10];
int		r_messaggia	[10];


int		c_tempo =1000;		// tempo ciclo e intervallo

// legge una stringa dal Serial Monitor
// Questa procedura serve a leggere
// i comandi inviati tramite il SERIAL MONITOR
// o i comandi che arrivano da ARDUINO

boolean SerialRead(char sbuffer[])
{
	int i =0;
  
	if (Serial.available() > 0)
	{            
		while (Serial.available() > 0) 
		{
			sbuffer[i]=Serial.read();
			delay(10);
			i++;      
		}
	}
	sbuffer[i]='\0';
	return (i);
}


void  Analizza_Sirena()
{
	int ix;
	int iy;
	int	iz;
	
	char	appo[30];
	char	a_azione[15];
	int		a_cicli =0;
	int		a_tempo_ciclo =0;
	int		a_tempo_intervallo =0;
	
  
	for (ix =17, iy =iz =0, *appo ='\0'; smsbuffer[ix]; ix++)
	{
		if (!isPrintable(smsbuffer[ix]))
			continue;
		if (smsbuffer[ix] =='|')
		{
			appo[iy] ='\0';
			if (!iz)
			{
				if (strlen(appo) <16)
					strcpy(a_azione, appo);
				else
					strcpy(a_azione, "NON_TROVATA");
			}
			else	if(iz ==1)
				a_cicli =atoi(appo);
			else	if(iz ==2)
				a_tempo_ciclo =atoi(appo);
			iy =0;
			iz++;
			*appo ='\0';
		}
		else
			appo[iy++] =smsbuffer[ix];
	}
	appo[iy] ='\0';
	if (iz ==1)
		a_cicli =atoi(appo);
	else	if(iz ==2)
		a_tempo_ciclo =atoi(appo);
	else	
		a_tempo_intervallo =atoi(appo);

	for (ix =0; ix <7;ix++)
	{
		if (!strcmp(w_azione[ix], a_azione))
		{
			w_cicli[ix] =a_cicli;
			w_tempo_ciclo[ix] =a_tempo_ciclo;
			w_tempo_intervallo[ix] =a_tempo_intervallo;
			Serial.print(ix);
			Serial.print(" - ");
			Serial.print(a_azione);
			Serial.print(" - ");
			Serial.print(a_cicli);
			Serial.print(" - ");
			Serial.print(a_tempo_ciclo);
			Serial.print(" - ");
			Serial.println(a_tempo_intervallo);
			break;
		}
	}
}


void Suona_Sirena()
{
	int	ix;
	int	iy;
	int	iz;
	
	digitalWrite(SIRENA, LOW);

	if (i_aiuto)
		ix =6;
	else	if (allarme)
		ix =5;
	else	if (i_acceso)
		ix =0;
	else	if (i_spento)
		ix =1;
	else	if (i_notte)
		ix =2;
	else	if (i_tipo_1)
		ix =3;
	else	if (i_test)
		ix =4;
	else
		return;
	// Serial.print("Posizione e tempi -> ");
	// Serial.print(ix);
	// Serial.print(" - ");
	// Serial.print(w_cicli[ix]);
	// Serial.print(" - ");
	// Serial.print(w_tempo_ciclo[ix]);
	// Serial.print(" - ");
	// Serial.println(w_tempo_intervallo[ix]);
	
	for (iy =0; iy <w_cicli[ix]; iy++)
	{
		if (ix !=6 || w_tempo_ciclo[ix])
			digitalWrite(SIRENA, HIGH);

		if (w_tempo_ciclo[ix])
			delay(w_tempo_ciclo[ix]);
		else
			delay(c_tempo);
		digitalWrite(SIRENA, LOW);
		if ((iy +1) ==w_cicli[ix])
			continue;
		if (w_tempo_intervallo[ix])
			delay(w_tempo_intervallo[ix]);
		else
			delay(c_tempo);
	}
}

void	Azzera_Rubrica()
{
	int	ix;

	for (ix =0; ix <10; ix++)
	{
		*r_nome[ix] = *r_numero[ix] ='\0';
		r_telefona[ix] =r_messaggia[ix] =0;
	}
}

void	Analizza_Rubrica()
{
	int ix;
	int iy;
	int	iz;
	
	char	appo[51];
	char	a_nome[51];
	char	a_numero[16];
	int		a_telefona =0;
	int		a_messaggia =0;
 
	for (ix =18, iy =iz =0, *appo ='\0'; smsbuffer[ix]; ix++)
	{
		if (!isPrintable(smsbuffer[ix]))
			continue;
		if (smsbuffer[ix] =='|')
		{
			appo[iy] ='\0';
			if (!iz)
				strcpy(a_nome, appo);
			else	if(iz ==1)
			{ 
				if (strlen(appo) <20)
					strcpy(a_numero, appo);
				else
					sprintf(a_numero, "%-15.15s", appo);
			}
			else	if(iz ==2)
			{ 
				if (!strcmp(appo, "SI"))
					a_telefona =1;
			}
			else	if(iz ==3)
			{ 
				if (!strcmp(appo, "SI"))
					a_messaggia =1;
			}
			iy =0;
			iz++;
			*appo ='\0';
		}
		else
		{
			if (iy <50)
				appo[iy++] =smsbuffer[ix];
		}
	}
	appo[iy] ='\0';
	if(iz ==3)
	{ 
		if (!strcmp(appo, "SI"))
			a_messaggia =1;
	}

	for (ix =0; ix <10;ix++)
	{
		if (!*r_nome[ix]
		|| (!strcmp(r_nome[ix], a_nome))
		|| (!strcmp(r_numero[ix], a_numero)))
		{
			strcpy(r_nome[ix], a_nome);
			strcpy(r_numero[ix], a_numero);
			r_telefona[ix] =a_telefona;
			r_messaggia[ix] =a_messaggia;
			
			Serial.print(ix);
			Serial.print(" - ");
			Serial.print(a_nome);
			Serial.print(" - ");
			Serial.print(a_numero);
			Serial.print(" - ");
			Serial.print(a_telefona);
			Serial.print(" - ");
			Serial.println(a_messaggia);
			break;
		}
	}
}

void Reset_AlarmSystem()
{
	digitalWrite(SIRENA, LOW);
	digitalWrite(LED, LOW);     // spegne inizialmente il LED che indica l'allarme
	acceso = false;             // Indica che il LED non è acceso (allarme OFF)
	SMSInviato=false;           // Indica che se avrò un allarme invia un SMS di avviso
	Serial.println("allarme_spento");
	
	//Serial.println("arduino_sirena");
	allarme =0;
	led_status =0;
	indice_rubrica =0;

	i_spento =1;
	i_acceso =i_aiuto =i_notte =i_tipo_1 =i_test =0;
	i_messaggi =i_telefono =i_sirena =1;
	Suona_Sirena();
}

void Activate_AlarmSystem(int i_acc, int i_not,int i_ti1)
{
	digitalWrite(LED, HIGH);  // accende il LED simulando un allarme
	acceso = true;            // Indica che il LED è acceso (allarme ON => pushbutton schiacciato)
	SMSInviato=false;           // Indica che se avrò un allarme invia un SMS di avviso
	allarme =0;
	led_status =1;
	indice_rubrica =0;
	i_acceso =i_acc;
	i_notte =i_not;
	i_tipo_1 =i_ti1;
	i_spento =i_aiuto =i_test =0;
	if (i_acceso)
		Serial.println("allarme_generale");
	else	if (i_notte)
		Serial.println("allarme_notte");
	else	if (i_tipo_1)
		Serial.println("allarme_tipo_1");
	else
	{
		Serial.println("allarme_test");
		acceso = false;
		led_status =0;
		i_test =1;
	}
	Suona_Sirena();
}

void  sms_status(int  flagall)
{
	if (allarme && flagall)
		sprintf(smsbuffer, "Allarme in ALLARME --> %s", messaggio);
	if (!arduino)
	{
		// if (allarme)
			// sms.SendSMS(Mittente, smsbuffer); // restituisce true se invia l'SMS
		// else if (!acceso)
			// sms.SendSMS(Mittente,"Allarme SPENTO!"); // restituisce true se invia l'SMS
		// else	if (acceso)
			// sms.SendSMS(Mittente,"Allarme ACCESO!"); // restituisce true se invia l'SMS
		// else	if (i_notte)
			// sms.SendSMS(Mittente,"Allarme NOTTE!");
		// else	if (i_tipo_1)
			// sms.SendSMS(Mittente,"Allarme TIPO_1!");
		// else
			// sms.SendSMS(Mittente,"Allarme TEST!");
		// delay(500);
	}

	if (flagall)
		Serial.print("inviato messaggio a " +String(Mittente) +String(" : "));
	if (allarme && flagall)
		Serial.println(smsbuffer);
	else
	{
		if (allarme)
			Serial.println("Allarme in ALLARME");
		else	if (!acceso)
			Serial.println("allarme_spento");
		else	if (i_acceso)
			Serial.println("allarme_generale");
		else	if (i_notte)
			Serial.println("allarme_notte");
		else	if (i_tipo_1)
			Serial.println("allarme_tipo_1");
		else
			Serial.println("allarme_test");		
	}
}

int	ControlloMittente()
{
	int   ix;
	int   trovato =0;
	char  appo_mitt[20];
	char  appo_lett[20];
	
	//Serial.println("Controllo [tel. "+String(Mittente)+String("]: ") + String(smsbuffer)); 
	if (arduino)
		return(1);

	if (*Mittente =='+')
		sprintf(appo_mitt, "%s", &Mittente[3]);	//estraggo in numero senza l'eventuale +39
	else
		strcpy(appo_mitt, Mittente);

	for (ix =0; ix <10 && *r_nome[ix]; ix++)
	{
		if (*r_numero[ix] =='+')
			sprintf(appo_lett, "%s", &r_numero[ix][3]);
		else
			strcpy(appo_lett, r_numero[ix]);

		if (!strcmp(appo_lett, appo_mitt))
		{
			trovato =1;
			break;
		}
	}
	return(trovato);
}


void  Allarme_gen()
{
	int ix;
	int iy;
	
 	allarme = primo_giro =1;
	i_messaggi =i_telefono =i_sirena =1;
	Suona_Sirena();
	for (ix =8, iy =0; smsbuffer[ix]; ix++)
	{
		if (!isPrintable(smsbuffer[ix]))
			continue;
		if (smsbuffer[ix] =='|')
			break;
		if (smsbuffer[ix] =='~')
			messaggio[iy++] =' ';
		else
			messaggio[iy++] =smsbuffer[ix];
	}
	messaggio[iy] ='\0';
	Serial.print("arduino_allarme|IN ALLARME|");
	Serial.println(messaggio);
	if (smsbuffer[ix] =='|')
	{
		char	appo[30];
		int 	ic;
		
		ix++;
		*appo ='\0';
		
		for (iy = ic =0; smsbuffer[ix]; ix++)
		{
			if (!isPrintable(smsbuffer[ix]))
				continue;
			if (smsbuffer[ix] =='|')
			{
				appo[iy] ='\0';

				if (!strcmp(appo, "NO"))
				{
					if (!ic)
						i_messaggi =0;
					else	if (ic ==1)
						i_telefono =0;
					else	if (ic ==2)
						break;
				}
				ic++;
				iy =0;
				*appo ='\0';
				continue;
				
			}
			if (smsbuffer[ix] =='~')
				appo[iy++] =' ';
			else
				appo[iy++] =smsbuffer[ix];
		}
		if (ic)
		{
			appo[iy] ='\0';
			
			if (!strcmp(appo, "NO"))
				i_sirena =0;	
		}
	}
	Serial.print("azioni -> ");
	if (i_messaggi)
		Serial.print("-messaggi");
	if (i_telefono)
		Serial.print("-telefono");
	if (i_sirena)
		Serial.print("-sirena");
	Serial.println("-");
}

int messaggio_gen(int pos_rub)
{
	int rito =1;
  
	if (1 == gsm.GetPhoneNumber(pos_rub, Mittente)) // legge la rubrica e ritorna il numero in posizione sim_pos
		sms_status(1);
	else  rito =0;
  
	return(rito);
}

int telefona_atutti(int pos_rub)
{
	int rito =1;
  
	if (1 == gsm.GetPhoneNumber(pos_rub, Mittente)) // legge la rubrica e ritorna il numero in posizione sim_pos
	{
		Serial.println("Telefono a " +String(Mittente));
	//  gsm.call(Mittente, 20000);
		gsm.call("337634273", 20000);
	}
	else  rito =0;
  
	return(rito);
}

int 	Powerkey = 9;

void Carica_Sirena()
{
	Serial.println("arduino_sirena");
	
	for  (;;)
	{
		char position;

		position=SerialRead(smsbuffer);
		
		if (position) 
		{
			if (!strncmp(smsbuffer,"raspberry_sirena", 16)&& strlen(smsbuffer) > 17)
				Analizza_Sirena();
			else  if (!strncmp(smsbuffer,"raspberry_fine_sirena", 21))
			{
				Serial.println("Sirena caricata");
				break;
			}
		}
	}
}

void Carica_Rubrica()
{
	Azzera_Rubrica();
	Serial.println("arduino_rubrica");
	
	for  (;;)
	{
		char position;

		position=SerialRead(smsbuffer);
		
		if (position) 
		{
			if (!strncmp(smsbuffer,"raspberry_rubrica", 17)&& strlen(smsbuffer) > 18)
				Analizza_Rubrica();
			else  if (!strncmp(smsbuffer,"raspberry_fine_rubrica", 22))
			{
				Serial.println("Rubrica caricata");
				break;
			}
		}
	}
}

void power(void)
{
	digitalWrite(Powerkey, LOW); 
	delay(1000);               // wait for 1 second
	digitalWrite(Powerkey, HIGH);
}

void setup() 
{  
	pinMode(LED, OUTPUT);       // imposta il pin digitale (13) come output
	pinMode(SIRENA, OUTPUT);       // imposta il pin digitale (13) come output 	
	pinMode(Powerkey, OUTPUT);   // initialize the digital pin as an output.  
	Serial.begin(9600);
	Serial.println("ATTIVAZIONE SCHEDA TELEFONICA");
	mySwitch.enableReceive(0);  // Receiver on inerrupt 0 => that is pin #2

	while(GSMstarted ==false)
	{
		if ( gsm.begin(9600) )
		{
			Serial.println("STATUS Modulo GSM = PRONTO ");
			Serial.println("loop : attesa raspberry_fine_sirena ");
			Serial.println("loop : attesa raspberry_fine_rubrica");
			GSMstarted=true;  
		}
		else
		{
			Serial.println(" => STATUS Modulo GSM = INATTIVO");
			delay(1000);
			power();                     //power on the sim808 or power down the sim808
		}
	}
	Carica_Sirena();
	Carica_Rubrica();
	Reset_AlarmSystem();
}  

long	tempoPassato =0;
long	intervallo =1000;	//tempo di attesa tra le letture sms 
  
void loop() 
{ 
	char position;

	arduino =0;
  
	if (GSMstarted) // Se il sistema GSM si è attivato
	{
		position=SerialRead(smsbuffer);
		if (position)
		{
			arduino =1;
			strcpy(Mittente, "Arduino/Seriale");
		}
    	else	if (millis() <tempoPassato 
				|| ((millis() -tempoPassato) >intervallo))
    	{
			// Serial.print(millis());
			// Serial.print(" <- millis-tempo -> ");
			// Serial.print(tempoPassato);
			// Serial.println(" => leggo SMS");
			tempoPassato =millis();
			position = sms.IsSMSPresent(SMS_ALL); // Valore da 1..20
			if (position)
        		sms.GetSMS(position, Mittente, smsbuffer, 60);
    	}
		if (position && ControlloMittente()) 
		{
			if (!strncmp(smsbuffer,"Spegni", 6))
				Reset_AlarmSystem();
			else  if (!strncmp(smsbuffer,"carica_sirena", 13))
				Carica_Sirena();
			else  if (!strncmp(smsbuffer,"carica_rubrica", 14))
				Carica_Rubrica();
			else  if (!strncmp(smsbuffer,"Accendi", 7))
				Activate_AlarmSystem(1, 0, 0);
			else  if (!strncmp(smsbuffer,"Home", 4))
				Activate_AlarmSystem(0, 1, 0);
			else  if (!strncmp(smsbuffer,"Tipo_1", 6))
				Activate_AlarmSystem(0, 0, 1);
			else  if (!strncmp(smsbuffer,"Test", 4))
				Activate_AlarmSystem(0, 0, 0);
			else  if (!strncmp(smsbuffer,"Stato", 5))
				sms_status(0);
			else  if (!strncmp(smsbuffer,  "Ti ho chiamato 1 volta,", 23))
			{
				Serial.println("[telefonata ricevuta da "+String(Mittente)+String("]"));
				Reset_AlarmSystem();
			}				
			else  if (!strncmp(smsbuffer,"Aiuto", 5))
			{
				Serial.println("allarme_aiuto");
				if (w_tempo_ciclo[6])
					strcpy(smsbuffer, "Allarme,AIUTO AIUTO AIUTO|SI|SI|SI");
				else
					strcpy(smsbuffer, "Allarme,AIUTO AIUTO AIUTO|SI|SI|NO");
				i_aiuto =1;
				i_acceso =i_spento =i_notte =i_tipo_1 =i_test =0;
				Allarme_gen();
			}
			else  if (!strncmp(smsbuffer,"Allarme", 7) && strlen(smsbuffer) > 8 && acceso)
				Allarme_gen();
			
			else
			{
				Serial.print("Comando Ricevuto [tel. "+String(Mittente)+String("]: ") + String(smsbuffer));
				Serial.println(" => Usare [Accendi] [NOTTE] [Test] [Spegni] [Stato] [Aiuto] [Allarme,....]");
			}
      
			if (!arduino)
			{
				sms.DeleteSMS(position); // Elimina l'SMS appena analizzato
				delay(500);
			} 
		}
		else  if (position)
		{
			Serial.print(" => Numero non autorizzato! ");
			Serial.print(Mittente);
			Serial.print(" - ");
			Serial.println(smsbuffer);
			sms.DeleteSMS(position); // Elimina l'SMS appena analizzato
			delay(500);
		}
	}
	else 
	{
		Serial.println("GSM INATTIVO");
		return; // se il GSM non parte il sistema di allarme non viene gestito.
	}
	if (allarme && !primo_giro)
	{
		arduino =0;
    
		if (led_status)
			digitalWrite(LED, LOW);     // spegne inizialmente il LED che indica l'allarme
		else
			digitalWrite(LED, HIGH);     // spegne inizialmente il LED che indica l'allarme
		
		if (led_status)
			led_status =0;
		else
			led_status =1;
		
		if (i_sirena)
			digitalWrite(SIRENA, HIGH);
		
		if (i_messaggi && SMSInviato ==false)
		{
			if (!messaggio_gen(++indice_rubrica))
			{
				SMSInviato =true;
				indice_rubrica =0;
			}
		}
		else	if (i_telefono)
		{
			if (!telefona_atutti(++indice_rubrica))
			indice_rubrica =0;
		}
	}
	else	if (allarme)
		primo_giro =0;
	
	if (!allarme
		&& (acceso || i_test)
		&& mySwitch.available())
	{
		int value = mySwitch.getReceivedValue();
    
		if (value == 0)
		{
			Serial.print("Unknown encoding");
		}
		else
		{
			Serial.print("arduino_433|");
			Serial.println( mySwitch.getReceivedValue() );
		}

		mySwitch.resetAvailable();
	}
}
