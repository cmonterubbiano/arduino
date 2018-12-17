/* -----------------------------------------------------------------------------------
 Scopo del programma è simulare la gestione di un Allarme (simulato con la pressione
 di un PUSHBUTTON). L'allarme puo' essere disinserito con un opportuno SMS inviato
 da un cellulare autorizzato
 Questa dovrebbe eesere l'ultima versione di GIORGIA
 ed e' contenuta nella cartella SIM900 
 attenzione alla libreria SIM900 differente tra MEGA e UNO
----------------------------------------------------------------------------------- */

#include "SIM900.h"
#include <SoftwareSerial.h>   // Necessaria alla libreria gsm.h richiamata in sms.h
#include "sms.h"              // Libreria per la gestione degli SMS
SMSGSM sms;
   
#define LED 13			// LED collegato al pin digitale 13 
 
int		val = 0;		// val conserva lo stato attuale del pin di input (7) 
int		prec_val;		// Variabile per valutare lo stato nel precedente Loop
boolean	acceso;			// Acceso = true  => il LED è acceso
boolean GSMstarted=false;
boolean SMSInviato;
char	smsbuffer[60];
char	messaggio[60];
char	Mittente[20];
int		allarme =0;
int		arduino =0;
int		led_status =0;
int		indice_rubrica =0;	//  indice inizio rubrica allarme

// legge una stringa dal Serial Monitor
// Questa procedura serve a leggere
// i comandi inviati tramite il SERIAL MONITOR
// o i comandi che arrivano da ARDUINO
boolean SerialRead(char s[])
{
	int i=0;
	
	if (Serial.available() > 0)
	{            
		while (Serial.available() > 0) 
		{
			s[i]=Serial.read();
			delay(10);
			i++;      
		}
	}
	s[i]='\0';
	return (i!=0);
}

void Reset_AlarmSystem()
{
	digitalWrite(LED, LOW);     // spegne inizialmente il LED che indica l'allarme
	acceso = false;             // Indica che il LED non è acceso (allarme OFF)
	SMSInviato=false;           // Indica che se avrò un allarme invia un SMS di avviso
	Serial.println("00==> Sistema di allarme spento");
	allarme =0;
	led_status =0;
	indice_rubrica =0;
}

void Activate_AlarmSystem()
{
	digitalWrite(LED, HIGH);  // accende il LED simulando un allarme
	acceso = true;            // Indica che il LED è acceso (allarme ON => pushbutton schiacciato)
	SMSInviato=false;           // Indica che se avrò un allarme invia un SMS di avviso
	Serial.println("01==> Sistema di allarme acceso");
	allarme =0;
	led_status =1;
	indice_rubrica =0;
}

void  sms_status(int	flagall)
{
	if (allarme	&& flagall)
		sprintf(smsbuffer, "Allarme in ALLARME --> %s", messaggio);
	if (!arduino)
	{
		// if (allarme)
			// sms.SendSMS(Mittente, smsbuffer); // restituisce true se invia l'SMS
		// else	if (!acceso)
			// sms.SendSMS(Mittente,"Allarme SPENTO!"); // restituisce true se invia l'SMS
		// else
			// sms.SendSMS(Mittente,"Allarme ACCESO!"); // restituisce true se invia l'SMS
		delay(500);
	}

	if (flagall)
		Serial.print("inviato messaggio a " +String(Mittente) +String(" : "));
	if (allarme && flagall)
  	  Serial.println(smsbuffer);
	else
	{
		if (allarme)
			Serial.println("02==> Allarme in ALLARME");
		else
			Serial.println((!acceso ? "00==> Allarme SPENTO" : "01==> Allarme ACCESO"));		
	}
}

int	ControlloMittente()
{
	int		ix;
	int		iy;
	int		iz;
	int		trovato =0;
	char	appo_mitt[20];
	char	appo_lett[20];
	
	if (arduino)
		return(1);
	
	for (ix =iy =0; ix <20 && Mittente[ix]; ix++)
	{
		if (!ix && Mittente[ix] =='+')
		{
			ix +=2;	//estraggo in numero senza l'eventuale +39
			continue;
		}
		appo_mitt[iy++] =Mittente[ix];
	}
	appo_mitt[iy] ='\0';
	// Serial.print("appo_mitt ---> ");
	// Serial.println(appo_mitt);
//	gsm.call(Mittente, 20000);		// Come effettuare una chiamata a Mittente con attesa di fine telefonata di 20000 millisecondi
//	for (iz =1; ; iz++)	//Legge la rubrica da posizione 1 e smette alla prima occorrenza vuota
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
					ix +=2;	//estraggo in numero senza l'eventuale +39
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

void	Allarme_gen()
{
	int	ix;
	int	iy;
	
	for (ix =8, iy =0; smsbuffer[ix]; ix++)
	{
		if (!isPrintable(smsbuffer[ix]))
			continue;
		messaggio[iy++] =smsbuffer[ix];
	}
	messaggio[iy] ='\0';
	Serial.print("messaggio ----> ");
	Serial.println(messaggio);
	allarme =1;
}

int	messaggio_gen(int	pos_rub)
{
	int	rito =1;
	
	if (1 == gsm.GetPhoneNumber(pos_rub, Mittente)) // legge la rubrica e ritorna il numero in posizione sim_pos
			sms_status(1);
	else	rito =0;
	
	return(rito);
}

int	telefona_atutti(int	pos_rub)
{
	int	rito =1;
	
	if (1 == gsm.GetPhoneNumber(pos_rub, Mittente)) // legge la rubrica e ritorna il numero in posizione sim_pos
	{
		Serial.println("Telefono a " +String(Mittente));
	//	gsm.call(Mittente, 20000);
                gsm.call("337634273", 20000);
	}
	else	rito =0;
	
	return(rito);
}

void setup() 
{  
	pinMode(LED, OUTPUT);       // imposta il pin digitale (13) come output  

	Serial.begin(9600);

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
		}
	}
	Reset_AlarmSystem();
}  
  
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
		else
		{
			position = sms.IsSMSPresent(SMS_ALL); // Valore da 1..20
			if (position)
				sms.GetSMS(position, Mittente, smsbuffer, 60);
		}
		if (position && ControlloMittente()) 
		{
			if (strncmp(smsbuffer,"Spegni", 6)==0)
				Reset_AlarmSystem();
			else  if (strncmp(smsbuffer,"Accendi", 7)==0)
				Activate_AlarmSystem();
			else  if (!strncmp(smsbuffer,"Stato", 5))
				sms_status(0);
			else  if (!strncmp(smsbuffer,"Aiuto", 5))
			{
				strcpy(smsbuffer, "Allarme,AIUTO AIUTO AIUTO");
				Allarme_gen();
			}
			else  if (!strncmp(smsbuffer,"Allarme", 7) && strlen(smsbuffer) > 10 && acceso)
				Allarme_gen();
			else
			{
				Serial.print("Comando Ricevuto [tel. "+String(Mittente)+String("]: ") + String(smsbuffer));
				Serial.println(" => Usare [Accendi] [Spegni] [Stato] [Aiuto] [Allarme,....]");
			}
			
			if (!arduino)
			{
				sms.DeleteSMS(position); // Elimina l'SMS appena analizzato
				delay(500);
			} 
		}
		else	if (position)
			Serial.println(" => Numero non autorizzato!");
		
		if (!arduino)
		{
			sms.DeleteSMS(position); // Elimina l'SMS appena analizzato
			delay(500);
		} 
	}
	else	return; // se il GSM non parte il sistema di allarme non viene gestito.
	
	if (allarme)
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
}

