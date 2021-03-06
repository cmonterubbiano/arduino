#include <SPI.h>
#include <nRF24L01.h>
#define MIRF_PAYLOAD 32
#include "RF24.h"

#include <RCSwitch.h>
#include <IRremote.h>

IRsend irsend;

//You could also try 36,40 & 56 frequencies as a test NORMAL = 38
int khz = 38; //NB Change this default value as neccessary to the correct modulation frequency

RCSwitch mySwitch = RCSwitch(); //433

char msg[MIRF_PAYLOAD + 1];
int msgIndex = 0;
unsigned  long  time1;

// RF24 protocol
#define  CHISONO  4
RF24 radio(8,7);
byte addresses[][6] = {"clie1","clie1"};

// IR protocol
int numeroRipetizionePerQuestoIR = 3; 

// power ON intero grezzo  
unsigned int powerButton[] = {1304,376,1304,376,464,1212,1304,376,1300,376,468,1212,464,1212,468,1212,464,1208,472,1212,460,1212,1308,7088};

unsigned int highButton[] = {1308,376,1304,368,472,1208,1308,372,1308,368,468,1208,472,1208,1308,372,468,1208,468,1212,468,1208,472,7916};

unsigned int signal1 = 10;
unsigned int signal2 = 210;
unsigned int signal3 = 410;
unsigned int signal4 = 610;
unsigned int incremento = 0;

/*
 * 4294967295 valore max di un long unsiged
 * tracciato comandi
 * M = millisecondo x riconoscere il comando e renderlo unico
 * V = invio = 0, feedback =1 x comandi normali invio = 3, feedback = 4, x comandi ricevuti da 433
 * R = 1 numero che identifica quante volte il comando e' stato ripetuto
 * C = 1 numero che identifica il comando de eseguire/eseguito
 * I = 1 numero che identifica l'arduino di competenza 0 = raspberry
 * 
 * il tutto in un long unsigned secondo la formula seguente
 * 
 * Valore = (M * 10000) + (V * 1000) + (R * 100) + (C * 10) + I
 * 
 */
#define  NE  50
 // buffer comandi rimbalzati
unsigned  long rcomm[NE];  // comando

unsigned  long  diretto;  //  millisecondi comando diretto

// tempo di attessa dopo cui ripete il comando
#define ATTESA  1000

int led = 13;
int ix;

int comandoInRipetizione = 0;
int numeroRipetizioneInCorso = 0;
unsigned long millisProssimaEsecuzione = 0;

unsigned long millisProssimaRipetizioneAggiornamento = 0;
unsigned long millisPerTagPrimoAggiornamento = 0;
unsigned long tagFeesbackDiAggiornamentoAtteso = 0;
int numeroRipetizioneAggiornamentoInCorso = 0;
int numeroMassimoRipetizioniAggiornamento = 5;
int comandoInInvioAggiornamento = 0;
int attesaInSecondiPerUnaRipetizioneRF24 = 2;


unsigned long pacchetto;
byte  scheda;  

void setup()
{
  int ix;
  
  for (ix =0; ix <NE; rcomm[ix++] =0L);
  pinMode(9, OUTPUT);
  Serial.begin(9600);
  irsend.enableIROut(khz);
  
  // Setup and configure rf radio
  radio.begin();                          // Start up the radio
  radio.setAutoAck(1);                    // Ensure autoACK is enabled
  radio.setRetries(15,15);    // Max delay between retries & number of retries
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1,addresses[0]);
  
  radio.startListening();                 // Start listening
  radio.printDetails();                   // Dump the configuration of the rf unit for debugging  
  
  incremento  = (10000 * CHISONO) + 1000;
  signal1 = signal1 + incremento;
  signal2 = signal2 + incremento;
  signal3 = signal3 + incremento;
  signal4 = signal4 + incremento;  
  mySwitch.enableReceive(0);  // Receiver on inerrupt 0 => that is pin #2
  
}

void  scala_buffer() // cancella dal buffer il comando piu' vecchio
{
  int ix;

  for (ix =0; ix < (NE -1); rcomm[ix] =rcomm[(ix +1)], ix++);
  rcomm[ix] =0L;
}

void  send_pacchetto(int attesa)
{
  radio.stopListening();
  delay(attesa * 10);	
	if (!radio.write( &pacchetto, sizeof(unsigned long) )){  printf("failed.\n\r");  }
	radio.startListening();   
	Serial.print("- Ho spedito : ");
	Serial.println(pacchetto);
}

void  esegui_comando(int  com)    // comando da esguire raggi infrarossi
{
	Serial.print("- Eseguo comando : ");
	Serial.println(com);

	comandoInRipetizione = com;

	if(com == 1){powerOnAndHIgh();}
	if(com == 2){powerOnOff();}

}

void  cavoli_miei()
{
  int ix;

  for (ix =0; ix <NE; ix++) //  controllo esistenza comando
  {
    if ((rcomm[ix] /10000L) ==(pacchetto /10000L))
      break;
  }
  if (ix ==NE)      // comando nuovo
  {
    long wfeed =((pacchetto /1000L) %10L);
	long wtag =(pacchetto /10000L);
	long wfin =(pacchetto %1000L);

    if (rcomm[(NE -1)]) //  se il buffer e' pieno
		scala_buffer();
	  
    for (ix =0; rcomm[ix]; ix++); // cerca prima spazio libero sul buffer
		rcomm[ix] =pacchetto;

		
	if(wfeed >= 3){
        diretto =millis();
		// se è 3 è una ridondanza da un mio comando... non mi interessa..
		//se è un 4, forse lo stavo aspettando!! ma controlliamo per sicurezza.
		if(wfeed == 4){
			if(tagFeesbackDiAggiornamentoAtteso == wtag){
				// Siiiiiiiiiii ho ricevuto feedback di aggiornamento, posso sbloccare i comndi diretti a 433 MHz che a questo punto saranno bloccati
				Serial.print("- Ricevuto Feedback di AGGIORNAMENTO : ");
				Serial.println(pacchetto);
				millisProssimaRipetizioneAggiornamento = 0;
			}
			
		}
	} else {
		numeroRipetizioneInCorso =0;
		esegui_comando((pacchetto /10L %10L));
		// preparo feedback a 1
		pacchetto =(wtag *10000L + ( 1 * 1000L) + wfin);
		send_pacchetto(2);   //  ripete/rimbalza comando
	}	
		
		
	
	
  }
  else
  {
	  long	wfeed =((pacchetto /1000L) %10L);
    long  wtag =(rcomm[ix] /10000L);
    long  wfin =(pacchetto %1000L);

	  if (wfeed)		//	ritorno del feedback di una segnalazione diretta all'arduino tramirte 433
    {
      if (wfeed ==4)
      {
			  rcomm[ix] =(wtag *10000L + ( 1 * 1000L) + wfin);
			  diretto =0L;	// azzero la variabile che conteneva il tempo di trasmissione e bloccava comandi diretti al 433
      }
	  }
	  else
	  {
			pacchetto =(wtag *10000L + ( 1 * 1000L) + wfin);
			send_pacchetto(2);   //  ripete/rimbalza comando 
	  }
  }
}

void  controllo_pacchetto() // analizzo comando
{
  int ix;

  for (ix =0; ix <NE; ix++) //  controllo esistenza comando
  {
    if (rcomm[ix] ==pacchetto)
      break;
  }

  if (ix ==NE)      // comando nuovo
  {
	  if ((pacchetto %10L) == CHISONO)
		  cavoli_miei();
	  else
	  {
		  if (rcomm[(NE -1)]) //  se il buffer e' pieno
		  	scala_buffer();
		  for (ix =0; rcomm[ix]; ix++); // cerca prima spazio libero sul buffer
		  rcomm[ix] =pacchetto;
		  send_pacchetto(1);   //  ripete/rimbalza comando
	  }
  }
}

void loop()
{
  int  traok =0;
  int ix;

  *msg ='\0';

  if (Serial.available() > 0)
  {
    int  datoletto;
    unsigned long appot;
    datoletto = Serial.read();
    time1 =millis();
    switch (datoletto)
    {
      case '0':
              sprintf(msg, "%06ld%d%d%d%d", 0L, 0, 0, 1, 2);
              break;
      case '1':
              sprintf(msg, "%06ld%d%d%d%d", 1L, 0, 0, 2, 2);
              break;
      case '2':
              sprintf(msg, "%06ld%d%d%d%d", 1L, 0, 1, 2, 2);
              break;
      case '3':
              sprintf(msg, "%06ld%d%d%d%d", 3L, 0, 0, 4, 2);
              break;
      case '4':
              appot =(time1 *10000L /10000L);
              sprintf(msg, "%06ld%d%d%d%d", appot, 0, 0, 5, 2);
              break;
      case '5':
              sprintf(msg, "%06ld%d%d%d%d", 5L, 3, 0, 1, 1);
              break;
      case '6':
              sprintf(msg, "%06ld%d%d%d%d", 6L, 0, 0, 1, 4);
              break;
      case '7':
              sprintf(msg, "%06ld%d%d%d%d", 7L, 0, 0, 1, 5);
              break;
      case '8':
              sprintf(msg, "%06ld%d%d%d%d", 8L, 0, 0, 1, 6);
              break;
      case '9':
              appot =(time1 *10000L /10000L);
              sprintf(msg, "%06ld%d%d%d%d", appot, 0, 0, 1, 7);
              break;
    }
  }

  //Serial.println(msg);
    pacchetto =atol(msg);
    
	if (strlen(msg) ==10)
		controllo_pacchetto();

	
	// leggo RF 2.4 Ghz
    if( radio.available())
	{
		while (radio.available()) // While there is data ready
		{                                   
			radio.read( &pacchetto, sizeof(unsigned long) );         // Get the payload
		}    
		Serial.print("- Dati ricevuti extra : ");
		Serial.println(pacchetto);
		controllo_pacchetto();
	}	  

	
	// controllo se ho comandi fisici in attesa di ripetizione
	if(millisProssimaEsecuzione != 0){
		if(millis() > millisProssimaEsecuzione){
			esegui_comando(comandoInRipetizione);
		}
	}	
	
	
	// Ascolto 433 MHz
	if (mySwitch.available())
	{
		if(millisProssimaRipetizioneAggiornamento == 0) // solo se sono disponibile e non sto aspettando un FB da raspberry...
		  {  
			unsigned long value = mySwitch.getReceivedValue();
			
			if (value == 0)
			  Serial.println("Unknown encoding");
			else
			{  
			  Serial.print(F("Received "));
			  Serial.println( value );
			  
			  if(value == signal1)
				ricevutoComandoDiretto433(1,0);
			  if(value == signal2)
				ricevutoComandoDiretto433(2,0);
			  if(value == signal3)
				ricevutoComandoDiretto433(3,0);
			  if(value == signal4)
				ricevutoComandoDiretto433(4,0);
				
				
			// solo per TEST	
			/*
			  if(value == 12844035) // Allarme HOME
				ricevutoComandoDiretto433(1,0);
			  if(value == 12844044) // Allarme DISARMA
				ricevutoComandoDiretto433(4,0);
			*/
			}
			mySwitch.resetAvailable();
		  }	
	}
	
	// aggiungere controllo di una eventuale ripetizione della avvenuta esecuzione diretta 433
	// controllo se ho comandi fisici in attesa di ripetizione
	if(millisProssimaRipetizioneAggiornamento != 0){
		if(millis() > millisProssimaRipetizioneAggiornamento){
			ricevutoComandoDiretto433(comandoInInvioAggiornamento, (numeroRipetizioneAggiornamentoInCorso + 1));
		}
	}	
	
}


void ricevutoComandoDiretto433(int directCommand, int ripetizoneAggiornamento)
{
	// eseguo il comando fisicamente + aggiornero' Rasp + aspettero' il feedback
	
	
	Serial.print(F("lancio comando di AGGIORNAMENTO verso RASP comando "));
	Serial.println( directCommand );
	Serial.print(F("ripetizione AGGIORNAMENTO: "));
	Serial.println( ripetizoneAggiornamento );

	if(ripetizoneAggiornamento < numeroMassimoRipetizioniAggiornamento){
		if(ripetizoneAggiornamento == 0){
			// Se è il primo ciclo eseguo anche il comando fisicamente...
			comandoInRipetizione = directCommand;
			numeroRipetizioneInCorso = 0;
			millisPerTagPrimoAggiornamento = (millis() * 10000L /10000L);
			if(directCommand == 1){powerOnAndHIgh();}
			if(directCommand == 4){powerOnOff();}	
		}
		// lancio un comando di aggionramento:
		inviaComandoAggiornamentoARaspberry(millisPerTagPrimoAggiornamento,3,ripetizoneAggiornamento,directCommand,CHISONO);
		comandoInInvioAggiornamento = directCommand;
		numeroRipetizioneAggiornamentoInCorso = ripetizoneAggiornamento;
		
		// prossima ripetizione comando di aggiornamento
		millisProssimaRipetizioneAggiornamento = millis() + (attesaInSecondiPerUnaRipetizioneRF24 * 1000);
		
	} else {
		Serial.print(F("Feedback NON ricevuto... smetto di ripetere... ok panico! torno in acolto..." ));
		millisProssimaRipetizioneAggiornamento = 0;
	}

	
}

void inviaComandoAggiornamentoARaspberry(unsigned long tag, int feedBack, int ripetizione, int comando, int destinatario){
	
	// compongo messaggio di Aggiornamento
	sprintf(msg, "%06ld%d%d%d%d", tag, feedBack, ripetizione, comando, destinatario);
	pacchetto =atol(msg);
	// registro il TAG in attesa Feedback
	tagFeesbackDiAggiornamentoAtteso = tag;
	// invio aggiornamento
	send_pacchetto(0);
}


void powerOnAndHIgh()
{
	numeroRipetizioneInCorso++;
	if(numeroRipetizioneInCorso <= 1){
		// ON
		Serial.print("- STUFETTA: power ON ");
		digitalWrite(led, HIGH);
		irsend.sendStufetta(powerButton, sizeof(powerButton) / sizeof(int), khz);
		digitalWrite(led, LOW);
		millisProssimaEsecuzione = millis() + 2000; // per agganciare un powerHigh appresso
	} else {
		// HIGH
		Serial.print("- STUFETTA: power HIGH ");
		digitalWrite(led, HIGH);
		irsend.sendStufetta(highButton, sizeof(highButton) / sizeof(int), khz);
		digitalWrite(led, LOW);
		millisProssimaEsecuzione = 0;
	}
}


void powerOnOff()
{
	Serial.print("- STUFETTA: power OFF ");
    digitalWrite(led, HIGH);
    irsend.sendStufetta(powerButton, sizeof(powerButton) / sizeof(int), khz);
	digitalWrite(led, LOW);
	millisProssimaEsecuzione = 0;
}

