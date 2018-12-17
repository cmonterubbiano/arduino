/*
Se in rubrica non c'e' la posizione 185
ossia la prima posizione vuota risponde cosi
AT+CPBR=185,185

OK
Se voglio vedere dalla 1 alla 1
AT+CPBR=1,1

+CPBR: 1,"3312229982",129,"A Cancello casa"
OK
Se voglio vedere dalla 1 alla 3
AT+CPBR=1,3

+CPBR: 1,"3312229982",129,"A Cancello casa"
+CPBR: 2,"3318967748",129,"A Cancello luci"
+CPBR: 3,"42202",129,"Aggiorna SIM"
OK
Se voglio vedere dalla 2 alla 2
AT+CPBR=2,2

+CPBR: 2,"3318967748",129,"A Cancello luci"
OK
AT+CMGL="REC UNREAD"
AT+CMGL="REC READ"
AT+CMGL="ALL"

Per leggere da data e l'ora
AT+CCLK?
+CCLK: "16/11/09,19:50:43+01"
Per settarla
AT+CCLK="16/11/09,19:49:50+01"
Per cancellare un poszione in rubrica
AT+CPBW= 2    dove 2 e' la posizione
Per inserire un numero in rubrica
AT+CPBW=,"0039337634273",129,"Claudio"

AT+CGPSPWR=1  attiva    GPS
AT+CGPSPWR=0  disattiva GPS
AT+CGPSSTATUS?  Stato GPS
+CGPSSTATUS: Location 3D Fix  Risposta l'importante non sia NO FIX

AT+CGPSOUT=255  Attiva visualizzazione NMEA
AT+CGPSOUT=0    Disattiva visualizzazione NMEA







 */
 #include <SoftwareSerial.h>
 
SoftwareSerial GPRS(7, 8); //Seriale simulata per comunicare col modem
 
unsigned char buffer[64]; // buffer per i dati scambiati sulla seriale
int count=0;              // contatore dati nel buffer
 
void setup()
{
  GPRS.begin(9600);   // Baudrate di default del modem SIM900
  Serial.begin(9600); // Baudrate della seriale di Arduino 
}
 
void loop()
{
  if (GPRS.available())  // Se ci sono dati sulla seriale simulata (il modem ci invia dati)
  {
    while(GPRS.available()) // Finché ci sono dati disponibili
    {
      buffer[count++]=GPRS.read(); // Inserisce i byte nel buffer
      if(count == 64)break;
    }
    Serial.write(buffer,count);  // Scrive i dati ricevuti sulla seriale di Arduino
    clearBufferArray();  // Svuota il buffer
    count = 0;           
   }
  if (Serial.available()) // Se ci sono dati sulla seriale di Arduino
    GPRS.write(Serial.read()); // Inviamo i dati al modem
}
void clearBufferArray() // Funzione di supporto per svuotare il buffer
{
  for (int i=0; i<count;i++)
      buffer[i]=NULL;                  
}
