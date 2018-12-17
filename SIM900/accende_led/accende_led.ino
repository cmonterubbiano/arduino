/* -----------------------------------------------------------------------------------
 Accende e spegne un LED a secondo dell'SMS inviato:
 ON  => Accende il LED
 OFF => Spegne il LED
 Altro => Messaggio di errore sul Serial Monitor
 ---------------------------------------------------------------------------------- */
#include "SIM900.h"
#include <SoftwareSerial.h> // necessaria alla libreria gsm.h richiamata in sms.h
// #include "inetGSM.h"     // necessaria per l'uso di HTTP
#include "sms.h"            // Libreria per la gestione degli SMS

SMSGSM sms;
int led = 13; // Il Pin 13 è quello connesso al LED
int numdata;
boolean started=false;
char smsbuffer[160];
char Mittente[20];

void setup() 
{
  Serial.begin(9600);
  Serial.println("ESEMPIO INVIO/RICEZIONE SMS");
  pinMode(led, OUTPUT);   // imposta il pin 13 come pin di OUTPUT.
  digitalWrite(led, LOW); // spegne inizialmente il led
  // Inizializzo la connessione GSM impostando il baudrate
  // Per l'utilizzo di HTTP è opportuno usare 4800 baud o meno
  if ( gsm.begin(9600) )
  {
     Serial.println("STATUS Modulo GSM = PRONTO");
     started=true;  
  }
  else 
     Serial.println("STATUS Modulo GSM = INATTIVO");
};

// legge una stringa dal Serial Monitor
//   Questa procedura serve solo nella fase di TEST ovvero 
//   quando provo a simulare i comandi SMS tramite il SERIAL MONITOR
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
  s[i]='\0'; // Metto il terminatore di stringa
  return (i!=0);
}
// legge una stringa dal Serial Monitor
//    Questa è la versione proposta da Arduino Lab
// int readSerial(char result[])
// {
//    int i = 0;
//    while(1)
//    {
//       while (Serial.available() > 0)
//       {
//          char inChar = Serial.read();
//          if (inChar == '\n') // Line Feed
//          {
//             result[i] = '\0';
//             Serial.flush();
//             return 0;
//           }
//           if(inChar!='\r') // Scarta i Carriage Return
//           {
//              result[i] = inChar;
//              i++;
//            }
//       }
//    }
//  }

void loop() 
{
  char inSerial[50];
  char position;
  if (started)
  {
     // -- Righe commentate poichè utilizzate per nei test
     // strcpy(Mittente,"3380000000");
     // if (SerialRead(smsbuffer))
     // ------------------------------------------------------
    // Legge se ci sono messaggi disponibili sulla SIM Card
    // e li visualizza sul Serial Monitor.
    position = sms.IsSMSPresent(SMS_UNREAD); // Valore da 1..20
    if (position) 
    {
      // Leggo il messaggio SMS e stabilisco chi sia il mittente
      sms.GetSMS(position, Mittente, smsbuffer, 160);
      Serial.print("Comando Ricevuto [tel. "+String(Mittente)+String("]: ") + String(smsbuffer));
      if (strcmp(smsbuffer,"ON")==0)
      {
         digitalWrite(led, HIGH);   // Accende il LED impostando il voltaggio a HIGH
         Serial.println(" => Accendo il LED");
      }
      else if (strcmp(smsbuffer,"OFF")==0)
      {
         digitalWrite(led, LOW);    // Spengo il LED impostando il voltaggio a LOW
         Serial.println(" => Spengo il LED");
      }
      else if (strcmp(smsbuffer,"STATUS")==0)
      {
          if (digitalRead(led)==HIGH)
          {
             sms.SendSMS(Mittente, "STATUS: LED Acceso"); // restituisce true se invia l'SMS
             Serial.println(" => il LED e' Acceso");
          }
          else
          {
             sms.SendSMS(Mittente, "STATUS: LED Spento"); // restituisce true se invia l'SMS
             Serial.println(" => il LED e' Spento");
          }
      }
      else
         Serial.println(" => non riconosciuto!");
      sms.DeleteSMS(position); // Elimina l'SMS appena analizzato
    }
    delay(1000);
  }
};
