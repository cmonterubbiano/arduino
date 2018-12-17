#include <SIM900.h>
#include <SoftwareSerial.h>
#include <sms.h>
 
SMSGSM sms;
 
boolean started = false;
 
void setup() 
{
  
  // Impostiamo le connessioni seriali.
  Serial.begin(9600);

  Serial.println("GSM Shield testing.");
 
  if (gsm.begin(9600)) { // Connessione al modem avvenuta con successo
    Serial.println("\nstatus=READY");
    started=true;  
  }
  else 
    Serial.println("\nstatus=IDLE");
 
  if(started){
    // Inviamo l'SMS
    if (sms.SendSMS("337634273", "Hello from Arduino! e' arrivata la scheda GSM")) 
      Serial.println("\nSMS sent OK");
  }
 
};
 
void loop() 
{
  // Niente da fare qui :-)
};
