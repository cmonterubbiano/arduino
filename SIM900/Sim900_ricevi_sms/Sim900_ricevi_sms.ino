/* -----------------------------------------------------------------------------------
 Esempio creato partendo dal codice sul sito: 
 http://tronixstuff.com/2014/01/08/tutorial-arduino-and-sim900-gsm-modules/
 Mostra sul serial monitor tutti i caratteri in uscita sulla porta seriale del 
 modulo GSM e tra questi anche gli SMS
 ---------------------------------------------------------------------------------- */
#include <SoftwareSerial.h>
SoftwareSerial SIM900(7, 8);  // configuro TX/RX (2 TDX - 3 RDX)

char incoming_char=0;

void setup()
{
  Serial.begin(9600); // Ricordarsi di porre la connessione sulla finestra del Serial
                       // monitor a 19200
  SIM900.begin(9600); // setta la velocità di comunicazione sulla seriale del cellulare
  // delay(20000);     // eventuale attesa per il collegamento alla rete cellulare. .

  SIM900.print("AT+CMGF=1\r");  // Imposta la modalità SMS a testo
  delay(100);
  SIM900.print("AT+CNMI=2,2,0,0,0\r"); // trasferisce il contenuto di un nuovo messaggio 
                                       // SMS all'uscita seriale del modulo GSM.
  delay(100);
  SIM900.print("AT+CSCS=\"GSM\"\r"); // Imposta la codifica GSM
  delay(100);
  SIM900.print("AT+CMGL=\"REC UNREAD\"\r"); // Imposta la codifica GSM
  delay(100);
}

void loop()
{
  if(SIM900.available() >0)
  {
    incoming_char=SIM900.read(); // Legge il carattere sulla porta seriale del cellulare
    Serial.print(incoming_char); // e lo mostra sul serial monitor
  }
}
