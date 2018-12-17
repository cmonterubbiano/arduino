#define MIRF_PAYLOAD 4
#include <RCSwitch.h>
#include <DHT.h>
#include <IRremote2.h>

IRsend irsend;

//You could also try 36,40 & 56 frequencies as a test NORMAL = 38
int khz = 38; //NB Change this default value as neccessary to the correct modulation frequency

RCSwitch mySwitch = RCSwitch(); //433

char msg[MIRF_PAYLOAD + 1];
int msgIndex = 0;

// RF24 protocol
// CHISONO  1 = SALA
// CHISONO  2 = CAMERA SUD
// CHISONO  3 = CAMERA NORD
// CHISONO  4 = STUFETTA BAGNO SOTTO
#define  CHISONO  3

// IR protocol
int numeroRipetizionePerQuestoIR = 3; 

/*
  // mitsubishi 23 gradi ventola 2 solo riscladamento  
  unsigned long signal_1[] = {0xc4d36480, 0x410e0, 0xc425400, 0x0, 0x70000}; 
  // mitsubishi 23 gradi ventola 4 solo riscladamento
  unsigned long signal_2[] = {0xc4d36480, 0x410e0, 0xc225400, 0x0, 0x470000}; 
  // mitsubishi 26 gradi ventola 3 solo raffrescamento
  unsigned long signal_3[] = {0xc4d36480, 0x41850, 0x6cc2d400, 0x0, 0xdf0000}; 
  // mitsubishi power OFF
  unsigned long signal_4[] = {0xc4d36480, 0x10e0, 0xcc2d400, 0x0, 0x430000}; 
*/

unsigned int signal1 = 10;
unsigned int signal2 = 210;
unsigned int signal3 = 410;
unsigned int signal4 = 610;
unsigned int incremento = 0;

// humidity
#define DHTPIN 4     // what digital pin we're connected to

#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
unsigned long millisPerAggiornamentoUmidita = 2000;
// ogni 30 sec alternato
unsigned int millisPerIntervalloUmidita = 30000;
bool ultimoInvioTemp = false;

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

// tempo di attessa dopo cui ripete il comando
#define ATTESA  1000

int led = 13;
int ix;

int comandoInRipetizione = 0;
int numeroRipetizioneInCorso = 0;
unsigned long millisProssimaEsecuzione = 0;

int intervalloParsimoniosoPerRidondanzeTelecomandi = 7000;
unsigned long millisUltimaRipetizioneParsimonia = 0;
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
  
  incremento  = (10000 * CHISONO) + 1000;
  signal1 = signal1 + incremento;
  signal2 = signal2 + incremento;
  signal3 = signal3 + incremento;
  signal4 = signal4 + incremento;  
  mySwitch.enableReceive(0);  // Receiver on inerrupt 0 => that is pin #2
  mySwitch.enableTransmit(12);  
  dht.begin();
}

void  scala_buffer() // cancella dal buffer il comando piu' vecchio
{
  int ix;

  for (ix =0; ix < (NE -1); rcomm[ix] =rcomm[(ix +1)], ix++);
  rcomm[ix] =0L;
}

void  send_pacchetto(int attesa)
{
	delay(attesa * 10);	
	mySwitch.send(pacchetto, 24);
	delay(10);	
	mySwitch.send(pacchetto, 24);
	delay(10);	
	mySwitch.send(pacchetto, 24);
	
	Serial.print("- Ho spedito : ");
	Serial.println(pacchetto);
}

void  esegui_comando(int  com)    // comando da esguire raggi infrarossi
{
	Serial.print("- Eseguo comando : ");
	Serial.println(com);

	comandoInRipetizione = com;

	/*
	if(com == 1){condMitsubishiComando(signal_1);}
	if(com == 2){condMitsubishiComando(signal_2);}
	if(com == 3){condMitsubishiComando(signal_3);}
	if(com == 4){condMitsubishiComando(signal_4);}
	*/
	condMitsubishiComando(com);
}

void  cavoli_miei()
{
	int ix;
	long  wfeed =((pacchetto /1000L) %10L);
	long wfin =(pacchetto %1000L);

	Serial.println(F("Cavoli miei:"));
	
  for (ix =0; ix <NE; ix++) //  controllo esistenza comando
  {
    if ((rcomm[ix] /10000L) ==(pacchetto /10000L))
      break;
  }
  
	
	if (ix ==NE)      // TAG non esiste, comando NUOVO
  {
	long wtag =(pacchetto /10000L);
	
	
    if (rcomm[(NE -1)]) //  se il buffer e' pieno
		scala_buffer();
	  
    for (ix =0; rcomm[ix]; ix++); // cerca prima spazio libero sul buffer
	
	rcomm[ix] =pacchetto; // memorizzio sul buffer

		
	if(wfeed >= 3){
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
		numeroRipetizioneInCorso = 0;
		esegui_comando((pacchetto /10L %10L));
		// preparo feedback a 1
		pacchetto =(wtag *10000L + ( 1 * 1000L) + wfin);
		send_pacchetto(2);   //  ripete/rimbalza comando
	}	
		
		
	
	
  }
  else
  {
	  // SE il TAG già esiste
    long  wtag =(rcomm[ix] /10000L);

	if (wfeed)	//	ritorno del feedback di una segnalazione diretta all'arduino tramirte 433
    {
      if (wfeed ==4)
		  {
				rcomm[ix] =(wtag *10000L + ( 1 * 1000L) + wfin);
				
				if(tagFeesbackDiAggiornamentoAtteso == wtag){
					// Siiiiiiiiiii ho ricevuto feedback di aggiornamento, posso sbloccare i comndi diretti a 433 MHz che a questo punto saranno bloccati
					Serial.print("- Ricevuto Feedback di AGGIORNAMENTO : ");
					Serial.println(pacchetto);
					millisProssimaRipetizioneAggiornamento = 0;
				}				
		  }
	}
	else
	{
		// se è un comnado ma lo avevo già eseguito
		pacchetto =(wtag *10000L + ( 1 * 1000L) + wfin);
		send_pacchetto(2);   //  ripete feedback comando 
	}
  }
}

void  controllo_pacchetto() // analizzo comando
{
  int ix;
  long wfeed =((pacchetto /1000L) %10L);
  
  // eccezioni tipo cancello cossora:
  if( (pacchetto == 12844044)
	||(pacchetto == 12844224)  
	||(pacchetto == 12844035)  
	||(pacchetto == 13894668)  
  ){
	  ripetiSegnaleTelecomandiMaConParsimonia();
	  return;
  }
  
  
  // controllo coerenza pacchetto
  if((wfeed >4)||(wfeed == 2))return;

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
  int traok =0;
  int ix;

  *msg ='\0';


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
				// Se è lungo 8 probabilmente viene da Raspberry
				// Se è lungo 5 probabilmente viene da telecomando diretto
			  Serial.print(F("Received "));
			  Serial.println( value );

				// chiamata raspberry
  			  if(value > 9999999){
				Serial.println(F("8 cifre, proabilmente da RASPBERRY "));
				pacchetto = value;
				controllo_pacchetto();				  
			  }

			  
				// telecomando diretto
			  if(value < 100000){
				  if(value == signal1)
					ricevutoComandoDiretto433(1,0);
				  if(value == signal2)
					ricevutoComandoDiretto433(2,0);
				  if(value == signal3)
					ricevutoComandoDiretto433(3,0);
				  if(value == signal4)
					ricevutoComandoDiretto433(4,0);
			  }
				
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
		} else {
			// Considerare MILLIS resettabile a fine capienza
		}
	}	
	
	
	// controllo se è ora di trasmettere temperatura o umidità:
	
	
	if(millis() > millisPerAggiornamentoUmidita){
		sendSignalWithTemperatureHumdity();
	} else {
		// Vediamo Se millis ha resettato...
		if(millis() < (millisPerAggiornamentoUmidita - millisPerIntervalloUmidita)){
			millisPerAggiornamentoUmidita = 2000;
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
			millisPerTagPrimoAggiornamento = (millis() * 100000L /100000L);
			if(millisPerTagPrimoAggiornamento > 675){millisPerTagPrimoAggiornamento -= 675;}
			millisPerTagPrimoAggiornamento += 1000;
			
			/*
			if(directCommand == 1){condMitsubishiComando(signal_1);}
			if(directCommand == 2){condMitsubishiComando(signal_2);}
			if(directCommand == 3){condMitsubishiComando(signal_3);}
			if(directCommand == 4){condMitsubishiComando(signal_4);}
			*/
			condMitsubishiComando(directCommand);			
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
	sprintf(msg, "%4ld%d%d%d%d", tag, feedBack, ripetizione, comando, destinatario);
	pacchetto =atol(msg);
	// registro il TAG in attesa Feedback
	tagFeesbackDiAggiornamentoAtteso = tag;
	// invio aggiornamento
	send_pacchetto(0);
}


//void condMitsubishiComando(unsigned long signal[]) {
void condMitsubishiComando(int com) {
 
	numeroRipetizioneInCorso++;
	digitalWrite(led, HIGH);

	if(com == 1){irsend.sendHvacMitsubishi(HVAC_HOT, 25, FAN_SPEED_2, VANNE_AUTO_MOVE, false);}
	if(com == 2){irsend.sendHvacMitsubishi(HVAC_HOT, 25, FAN_SPEED_4, VANNE_AUTO_MOVE, false);}
	if(com == 3){irsend.sendHvacMitsubishi(HVAC_COLD, 27, FAN_SPEED_3, VANNE_AUTO_MOVE, false);}
	if(com == 4){irsend.sendHvacMitsubishi(HVAC_HOT, 25, FAN_SPEED_1, VANNE_AUTO_MOVE, true);}
	
	//irsend.sendMitsubishi(signal[0],signal[1],signal[2],signal[3],signal[4]);
	

	digitalWrite(led, LOW);
	
	if(numeroRipetizioneInCorso >= numeroRipetizionePerQuestoIR)
	{
		millisProssimaEsecuzione = 0;
	} else {
		millisProssimaEsecuzione = millis() + 5000;
	}
  
}


void ripetiSegnaleTelecomandiMaConParsimonia(){
	if((millisUltimaRipetizioneParsimonia + intervalloParsimoniosoPerRidondanzeTelecomandi) < millis()){
		Serial.println(F("comando speciale RIPETO perche passato intervallo parsimonia"));
		millisUltimaRipetizioneParsimonia = millis();
		send_pacchetto(0);
	} else {
		Serial.println(F("comando speciale NON ripetuto per parsimonia"));
		if(millisUltimaRipetizioneParsimonia > millis()){
			// Millis si è resettato, resetto pure il counter
			millisUltimaRipetizioneParsimonia = 0;
		}
	}
}


void sendSignalWithTemperatureHumdity(){
	int h;
	millisPerAggiornamentoUmidita = millisPerAggiornamentoUmidita + millisPerIntervalloUmidita;
	
	//è il momento di mandare temp o umidità?
	if(ultimoInvioTemp){
		ultimoInvioTemp = false;
		h = static_cast<int>(dht.readHumidity());
		if (isnan(h)){return;}
				
		Serial.print("Humidity: ");
		Serial.print(h);
		Serial.println(" %\t");		
		
		sprintf(msg, "%d%d%d%d", (CHISONO + 5), (h/10 %10), (h/1 %10), (((CHISONO + 5)+(h/10 %10)+(h/1 %10)) % 10) );
		pacchetto = atol(msg);
		//Serial.println(pacchetto);	
		//Serial.println(" *C ");	

		
	} else {
		ultimoInvioTemp = true;
		h = static_cast<int>(dht.readTemperature());
		if (isnan(h)){return;}
		
		Serial.print("Temperature: ");
		Serial.print(h);
		Serial.println(" *C ");		
		
		sprintf(msg, "%d%d%d%d", CHISONO, (h/10 %10), (h/1 %10), ((CHISONO+(h/10 %10)+(h/1 %10)) % 10) );
		pacchetto = atol(msg);
		//Serial.println(pacchetto);	
		//Serial.println(" *C ");	
	}
	mySwitch.send(pacchetto, 24);
	//mySwitch.resetAvailable();
	//send_pacchetto(0);
}
