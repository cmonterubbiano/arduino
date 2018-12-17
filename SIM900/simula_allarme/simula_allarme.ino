/* -----------------------------------------------------------------------------------
 Scopo del programma è simulare la gestione di un Allarme (simulato con la pressione
 di un PUSHBUTTON). L'allarme puo' essere disinserito con un opportuno SMS inviato
 da un cellulare autorizzato
----------------------------------------------------------------------------------- */
//#define AMBIENTEDIPROVA true     // Commentare quando si usa l'ambiente GSM
#include "SIM900.h"
#ifndef AMBIENTEDIPROVA          // Compilazione condizionale
   #include <SoftwareSerial.h>   // Necessaria alla libreria gsm.h richiamata in sms.h
   #include "sms.h"              // Libreria per la gestione degli SMS
   SMSGSM sms;
#endif
#define LED 13        // LED collegato al pin digitale 13  
#define BOTTONE 10     // pin di input collegato al pulsante  
int val = 0;          // val conserva lo stato attuale del pin di input (7) 
int prec_val;         // Variabile per valutare lo stato nel precedente Loop
boolean acceso;       // Acceso = true  => il LED è acceso
boolean GSMstarted=false;
boolean SMSInviato;
char smsbuffer[160];
char CellulareAutorizzato[20];
char Mittente[20];
int inizio =1;
int primavolta =0;

#ifdef AMBIENTEDIPROVA
// legge una stringa dal Serial Monitor
//   Questa procedura serve solo nella fase di TEST ovvero se 
//   i comandi sono inviati tramite il SERIAL MONITOR
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
#endif

void Reset_AlarmSystem()
{
  digitalWrite(LED, LOW);     // spegne inizialmente il LED che indica l'allarme
  acceso = false;             // Indica che il LED non è acceso (allarme OFF)
  SMSInviato=false;           // Indica che se avrò un allarme invia un SMS di avviso
  Serial.println(" => Sistema di allarme spento");
}

void Activate_AlarmSystem()
{
  digitalWrite(LED, HIGH);  // accende il LED simulando un allarme
  acceso = true;            // Indica che il LED è acceso (allarme ON => pushbutton schiacciato)
  Serial.println(" => Allarme in corso ...");
}

void  sms_status()
{
  #ifndef AMBIENTEDIPROVA          // Compilazione condizionale
//  sms.SendSMS(CellulareAutorizzato,(!acceso ? "Allarme SPENTO" : "Allarme ACCESO"));
  if (!acceso)
     sms.SendSMS(CellulareAutorizzato,"Allarme SPENTO!"); // restituisce true se invia l'SMS
  else
        sms.SendSMS(CellulareAutorizzato,"Allarme ACCESO!"); // restituisce true se invia l'SMS
     delay(500);
  #endif 
  Serial.print("inviato messaggio stato allarme : ");
  Serial.println((!acceso ? "Allarme SPENTO" : "Allarme ACCESO"));
}
void setup() 
{  
  pinMode(LED, OUTPUT);       // imposta il pin digitale (13) come output  
  pinMode(BOTTONE, INPUT);    // imposta il pin digitale (7) come input  
  // La parte del SERIAL MONITOR è solo per il TEST
  Serial.begin(9600);
  Serial.println("ESEMPIO GESTIONE ALLARME");
  Reset_AlarmSystem();
  strcpy(CellulareAutorizzato,"+39337634273");
  #ifdef AMBIENTEDIPROVA
      // -- Righe utilizzate per i test
      Serial.println(" => STATUS Ambiente TEST = PRONTO");
      GSMstarted=true; 
  #else
      // Inizializzo la connessione GSM impostando il baudrate
      // Per l'utilizzo di HTTP è opportuno usare 4800 baud o meno
      if ( gsm.begin(9600) )
      {
         Serial.println(" => STATUS Modulo GSM = PRONTO");
         GSMstarted=true;  
      }
      else 
         Serial.println(" => STATUS Modulo GSM = INATTIVO");
  #endif 
}  
  
void loop() 
{ 
  char inSerial[50];
  char position;
  if (GSMstarted) // Se il sistema GSM si è attivato
  {
      if (!SMSInviato && acceso) // se l'allarme è attivato per la prima volta invia l'SMS
      {
          #ifndef AMBIENTEDIPROVA          // Compilazione condizionale
//             sms.SendSMS(CellulareAutorizzato, "ALLARME ATTIVATO!"); // restituisce true se invia l'SMS
             delay(500);
          #endif 
          SMSInviato=true;
          Serial.println(" => SMS di Allarme inviato!");
      }
      #ifdef AMBIENTEDIPROVA                // Compilazione condizionale
          position=SerialRead(smsbuffer);   // Test
          strcpy(Mittente,"+39337634273"); // Usato nei test per valutare se il mittente è autorizzato
      #else
          // Controllo se ci sono degli SMS in ingresso. Se l'allarme è spento allora li ignoro
          position = sms.IsSMSPresent(SMS_ALL); // Valore da 1..20
      #endif 
      if (position) 
      {
          #ifndef AMBIENTEDIPROVA          // Compilazione condizionale
              sms.GetSMS(position, Mittente, smsbuffer, 160);
          #endif 
          Serial.print("Comando Ricevuto [tel. "+String(Mittente)+String("]: ") + String(smsbuffer));
          if (strncmp(Mittente,CellulareAutorizzato,13)==0)
          {  
              if ((strncmp(smsbuffer,"ALARM-OFF", 9)==0) && acceso)
                 Reset_AlarmSystem();
              else  if ((strncmp(smsbuffer,"ALARM-ON", 8)==0) && !acceso)
                 Activate_AlarmSystem();
              else  if (!strncmp(smsbuffer,"STATUS", 6))
                 sms_status();
              else
                   Serial.println(" => Comando non autorizzato!");
          }
          else
              Serial.println(" => Cellulare non autorizzato!");
          #ifndef AMBIENTEDIPROVA      // Compilazione condizionale
              sms.DeleteSMS(position); // Elimina l'SMS appena analizzato
              delay(500);
          #endif 
      }
  }
  else
     return; // se il GSM non parte il sistema di allarme non viene gestito.
  if (!acceso && primavolta)
  {
    val = digitalRead(BOTTONE);  // leggo il valore dell'input e lo conservo in val  
    if (val == HIGH)             // se il pushbutton è premuto parte l'allarme
      Activate_AlarmSystem();
      inizio =0;
      primavolta =0;
  }
}
