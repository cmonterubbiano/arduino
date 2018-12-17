
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
char	smsbuffer[60];
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

char	w_azione	[][11] ={  "ACCESO", "SPENTO", "NOTTE", "TIPO_1", "TEST", "IN_ALLARME", "AIUTO"};
int		w_cicli	[] ={  0, 0, 0, 0, 0, 0, 0};
int		w_tempo_ciclo	[] ={ 0, 0, 0, 0, 0, 0, 0};
int		w_tempo_intervallo	[] ={ 0, 0, 0, 0, 0, 0, 0};

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

void Leggi_Sirena()
{
	int	ix;
	
	for (ix =0; ix <7; ix++)
	{
		Serial.print("arduino_sirena|");
		Serial.println(w_azione[ix]);
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
	Serial.print(ix);
	Serial.print(" - ");
	Serial.print(w_cicli[ix]);
	Serial.print(" - ");
	Serial.print(w_tempo_ciclo[ix]);
	Serial.print(" - ");
	Serial.println(w_tempo_intervallo[ix]);
	
	for (iy =0; iy <w_cicli[ix]; iy++)
	{
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

void Reset_AlarmSystem()
{
	Leggi_Sirena();
	digitalWrite(LED, LOW);     // spegne inizialmente il LED che indica l'allarme
	acceso = false;             // Indica che il LED non è acceso (allarme OFF)
	SMSInviato=false;           // Indica che se avrò un allarme invia un SMS di avviso
	Serial.println("allarme_spento");
	allarme =0;
	led_status =0;
	indice_rubrica =0;

	i_spento =1;
	i_acceso =i_aiuto =i_notte =i_tipo_1 =i_test =0;
	Suona_Sirena();
}

void Activate_AlarmSystem(int i_acc, int i_not,int i_ti1)
{
//digitalWrite(LED, HIGH);  // accende il LED simulando un allarme
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
		Serial.println("allarme_controllo");
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
		if (allarme)
			sms.SendSMS(Mittente, smsbuffer); // restituisce true se invia l'SMS
		else if (!acceso)
		sms.SendSMS(Mittente,"Allarme SPENTO!"); // restituisce true se invia l'SMS
		else
		sms.SendSMS(Mittente,"Allarme ACCESO!"); // restituisce true se invia l'SMS
		delay(500);
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
			Serial.println("allarme_controllo");		
	}
}

int ControlloMittente()
{
	int   ix;
	int   iy;
	int   iz;
	int   trovato =0;
	char  appo_mitt[20];
	char  appo_lett[20];
  
	if (arduino)
		return(1);

	for (ix =iy =0; ix <20 && Mittente[ix]; ix++)
	{
		if (!ix && Mittente[ix] =='+')
		{
			ix +=2; //estraggo in numero senza l'eventuale +39
			continue;
		}
		appo_mitt[iy++] =Mittente[ix];
	}
	appo_mitt[iy] ='\0';
	// Serial.print("appo_mitt ---> ");
	// Serial.println(appo_mitt);
	//  gsm.call(Mittente, 20000);    // Come effettuare una chiamata a Mittente con attesa di fine telefonata di 20000 millisecondi
	//  for (iz =1; ; iz++) //Legge la rubrica da posizione 1 e smette alla prima occorrenza vuota
	for (iz =(indice_rubrica +1); ; iz++)
	{
		byte  sim_pos =iz;
		char  sim_phone_num[20];

		if (1 == gsm.GetPhoneNumber(sim_pos, sim_phone_num)) // legge la rubrica e ritorna il numero in posizione sim_pos
		{
			for (ix =iy =0; ix <20 && sim_phone_num[ix]; ix++)
			{
				if (!ix && sim_phone_num[ix] =='+')
				{
					ix +=2; //estraggo in numero senza l'eventuale +39
					continue;
				}
				appo_lett[iy++] =sim_phone_num[ix];
			}
			appo_lett[iy] ='\0';
			// Serial.print("iz---> ");
			// Serial.print(iz);
			// Serial.print(" - appo_lett ---> ");
			// Serial.println(appo_lett);
			if (!strcmp(appo_lett, appo_mitt))
			{
				trovato =1;
				break;
			}
		}
		else break;
	}
	return(trovato);
}

void  Allarme_gen()
{
	int ix;
	int iy;
	
 	allarme = primo_giro =1;
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
			Serial.println("00=> STATUS Modulo GSM = PRONTO");
			GSMstarted=true;  
		}
		else
		{
			Serial.println(" => STATUS Modulo GSM = INATTIVO");
			delay(1000);
			power();                     //power on the sim808 or power down the sim808
		}
	}
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
			else  if (!strncmp(smsbuffer,"raspberry_sirena", 16)&& strlen(smsbuffer) > 17)
				Analizza_Sirena();
			else  if (!strncmp(smsbuffer,"Accendi", 7))
				Activate_AlarmSystem(1, 0, 0);
			else  if (!strncmp(smsbuffer,"Home", 4))
				Activate_AlarmSystem(0, 1, 0);
			else  if (!strncmp(smsbuffer,"Tipo_1", 6))
				Activate_AlarmSystem(0, 0, 1);
			else  if (!strncmp(smsbuffer,"Controllo", 9))
				Activate_AlarmSystem(0, 0, 0);
			else  if (!strncmp(smsbuffer,"Stato", 5))
				sms_status(0);
			else  if (!strncmp(smsbuffer,"Aiuto", 5))
			{
				Serial.println("allarme_aiuto");
				strcpy(smsbuffer, "Allarme,AIUTO AIUTO AIUTO");
				i_aiuto =1;
				i_acceso =i_spento =i_notte =i_tipo_1 =i_test =0;
				Allarme_gen();
			}
			else  if (!strncmp(smsbuffer,"Allarme", 7) && strlen(smsbuffer) > 8 && acceso)
				Allarme_gen();
			else
			{
				Serial.print("Comando Ricevuto [tel. "+String(Mittente)+String("]: ") + String(smsbuffer));
				Serial.println(" => Usare [Accendi] [NOTTE] [Controllo] [Spegni] [Stato] [Aiuto] [Allarme,....]");
			}
      
			if (!arduino)
			{
				sms.DeleteSMS(position); // Elimina l'SMS appena analizzato
				delay(500);
			} 
		}
		else  if (position)
		{
			Serial.println(" => Numero non autorizzato!");
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
//digitalWrite(LED, HIGH);     // spegne inizialmente il LED che indica l'allarme
		if (led_status)
			led_status =0;
		else
			led_status =1;
		if (SMSInviato ==false)
		{
			if (!messaggio_gen(++indice_rubrica))
			{
				SMSInviato =true;
				indice_rubrica =0;
			}
		}
		else
		{
			if (!telefona_atutti(++indice_rubrica))
			indice_rubrica =0;
		}
	}
	else	if (allarme)
		primo_giro =0;
	if ((acceso || i_test)
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
