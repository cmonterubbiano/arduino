/*
 ricevitore SMS

 Questo disegno, per lo scudo Arduino GSM, attende un messaggio SMS
 e lo visualizza tramite la porta seriale.

 Circuito:
 * Scudo GSM collegata al e Arduino
 * SIM card in grado di ricevere messaggi SMS

 creato 25 Feb 2012
 da Javier Zorzano / TD

 Questo esempio è di dominio pubblico.

 http://www.arduino.cc/en/Tutorial/GSMExamplesReceiveSMS

*/

// Includere la libreria GSM
#include <GSM.h>

// PIN numero per la SIM
#define PinNumber ""

// Inizializza le istanze della biblioteca
GSM gsmAccess;
GSM_SMS sms;

// Array per contenere il numero di un SMS è retreived da
char senderNumber [20];

void setup () {
  // Inizializza la comunicazione seriale e aspettare che la porta per aprire:
  Serial.begin (9600);
  while (! Serial) {
    ; // Aspettare per la porta seriale per la connessione. Necessario per porta USB nativa solo
  }

  Serial.println ( "Messaggi SMS ricevitore");

  // Stato della connessione
  boolean NONCONNESSO = true;

  // Collegamento Inizio GSM
  while (NONCONNESSO) {
    if (gsmAccess.begin (PinNumber) == GSM_READY) {
      NONCONNESSO = false;
    } else  {
      Serial.println ( "Non connesso");
      delay (1000);
    }
  }

  Serial.println ( "GSM inizializzato");
  Serial.println ( "In attesa di messaggi");
}

void loop () {
  char c;

  // Se ci sono SMS disponibili ()
  if (sms.available ()) {
    Serial.println ( "Messaggio ricevuto da:");

    // Ottenere numero remoto
    sms.remoteNumber (senderNumber, 20);
    Serial.println (senderNumber);

    // Un esempio di smaltimento messaggio
    // Tutti i messaggi che iniziano con # deve essere eliminata
    if (sms.peek () == '#') {
      Serial.println ( "SMS scartate");
      sms.flush ();
    }

    // Leggi byte del messaggio e stamparli
    while (c = sms.read ()) {
      Serial.print (c);
    }

    Serial.println ( "\ Nend del Messaggio");

    // Elimina il messaggio dalla memoria del modem
    sms.flush ();
    Serial.println ( "Messaggio cancellato");
  }

  delay (1000);

}
