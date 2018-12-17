/* *** *** ***
Policy di Utilizzo
Solo per uso privato. Ogni uso commerciale dei codici forniti dal sottoscritto sarà ritenuto
furto di proprietà intellettuale, perseguibile a norma di legge. 
Progetto realizzato da Gianni Favilli www.giannifavilli.it
Il codice sorgente (sketch) di Arduino sono di proprietà intellettuale del sottoscritto. 
Ogni tipo di riproduzione o divulgazione è vietata. 
Il committente si impegna a non divulgare a terzi i dati e le sorgenti realizzati
per il suddetto progetto in quanto proprietà intellettuale di Gianni Favilli
anche in seguito di fine collaborazione tra le parti.
Gianni Favilli
*** *** *** */
 
/* *** NB NELLO SKETCH SONO STATI INSERITI DELIBERATAMENTE UN PAIO DI ERRORI.
LA IDE IN FASE DI COMPILAZIONE VI AIUTERA' AD INDIVIDUARLI *** */
 
#include <String.h>
#include <SPI.h>
#include <Ethernet.h>
 
/* ***
COMPILARE CON ARDUINO 1.0 o successive
controllare un attuatore via web
con sensori di temperatura e luce
ARDUINO UNO
ETHERNET SHIELD
by GianniFavilli.it - www.giannifavilli.it
*** */
 
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // mac address
byte ip[] = { 192, 168, 1, 201 }; // ip arduino internet in
byte gateway[] = { 192, 168, 1, 201 }; // internet access via router
byte subnet[] = { 255, 255, 255, 255 }; //subnet mask
EthernetServer server(80); //server port
 
int outPin = 9; // pin attuatore
String readString; //string
boolean LEDON = false; // flag status attuatore
 
/*termistore*/
float temp; // variabile da calcolare
float volt; // variabile da calcolare sul sensore di temperatura
float tempPin = 1;   // pin analogico IN temperature sensor
int ledPintemp = 13; // pin led termistore
float tempreg = 25.0; // temperatura di controllo in celsius
/*end termistore*/
 
/*fotoresistore*/
int light; // variabile da calcolare
int ledPinlux = .12; // pin led fotoresistenza
int photoresistor = 0; // pin analogico IN fotoresistore
/*end fotoresistore*/
 
void setup(){
Ethernet.begin(mac, ip, gateway, subnet);
pinMode(outPin, OUTPUT);
pinMode(ledPinlux, OUTPUT);
Serial.begin(9600);
}
 
void loop(){
 
  /*inizio calcolo temperatura*/
  temp = ((5 * analogRead(tempPin) * 100.0 ) / 1024) - 50; // Codice ottimizzato
 
  /*
  (grazie giulio400 e jumpjack)
  volt = ( analogRead(tempPin) * 5) / 1024; // calcolo il valore letto e lo trasformo in valore di tensione
  temp = (volt - 0.5) * 100; // conversione MCP9700A
  */
  /*fine calcolo temperatura*/
 
  /*inizio luce*/
  light = analogRead(photoresistor);
  light = constrain(light, 0, 1023); // limiti dei valori tra 0 e 100
  light = map(light, 0, 150, 255, 0);
  /*fine luce*/
 
/*inizio client*/
EthernetClient client = server.available();
if (client) {
  boolean currentLineIsBlank = true;
  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
        readString.concat(c); //memorizzo ogni carattere della stringa
      //if HTTP request has ended
      if (c == '\n' && currentLineIsBlank) {
         Serial.print(readString);
        if(readString.indexOf("L=1") > 0) {// lettura del valore se il LED si deve accendere
          // il led sarà acceso
          digitalWrite(outPin, HIGH); // accendo il led
          LEDON = true;
          Serial.print("ON pin ");
          Serial.println(outPin);
          }
          else
          {
          //il led sarà spento
          digitalWrite(outPin, LOW); //sengo il led
          LEDON = false;
          Serial.print("OFF pin ");
          Serial.println(outPin);
        }
 
        // COSTRUZIONE PAGINA HTML
        client.println("HTTP/1.1 200 OK.....");
        client.println("Content-Type: text/html");
        client.println();
        // inizializzo pagina (da togliere se uso ajax)
        client.print("<html><head><title>ARDUINO Controllo Led via WEB</title><meta http-equiv='Content-Type' content='text/html; charset=iso-8859-1' ></head><body>");
        //inizai il body
        client.println("<div style='width:360px; height:640px;'>"); //risoluzione per nokia 5800 640x360
        client.println("<h1>STATUS SENSORI</h1><hr />");
        //Scrive sul browser il valore del termistore
        client.println("<p>TEMPERATURA = ");
        client.print(temp);
        client.println(" C <br /></p>");
      if (temp < tempreg) {
        // scrive sul web freddo se il valore del termistore è basso
        client.print("<p><strong>FREDDO</strong></p>");
        }
      else {
        // scrive sul web caldo se il valore del termistore è alto
        client.print(" <p><strong>CALDO</strong></p>");
    }  
 
		//Scrive sul browser il valore della fotoresistenza
            client.println("<p>LUCE = ");
            client.print(analogRead(light));
            client.println("<br /></p>");
      if (light < 150) {
          // scrive sul web luce se il valore della fotoresistenza è alto
          client.print("<p><strong>LUCE</strong></p>");
        }
      else {
          // scrive sul web buio se il valore della fotoresistenza è basso
          client.print(" <p><strong>BUIO</strong></p>");
        }
        // link per aggiornare pagina e valori
        client.print("<h2>AGGIORNA SENSORI: <a href=''>CHECK</a></h2><hr />");
 
        client.println("<h1>CONTROLLO ATTUATORI via internet</h1>");
        client.println("<hr />");
        client.print("<h1>PIN control n. ");
        client.print(outPin);
        client.println("</h1>");
        client.println("<br />");
        //scrivo il LED status
        client.print("<font size='5'>PIN status: ");
          if (LEDON) {
              client.println("<span style='color:green; font-weight:bold;'>ON</span></font>");
            }
            else
            {
              client.println("<span style='color:grey; font-weight:bold;'>OFF</span></font>");
          }
        client.print("<h2><a href='/?L=1'>ACCENDI</a> | <a href='/?L=0'>SPEGNI</a></h2>");
 
        // firma
        client.println("<hr />");
        client.print("<h4>Visita <a href='http://www.giannifavilli.it' target='_blank' />www.giannifavilli.it</a></h4>");
        client.println("<hr />");
 
        // chiudo il div
        client.println("</div>");
        // chiudo pagina da togliere se uso ajax
        client.println("</body></html>");
 
        // pulisco la stringa per la successiva lettura
        readString="";
        //fermo il client
        client.stop();
 
        } //if c == /n
    } // if client available
  } // while client connesso
} // FINE if client
 
        if (temp < tempreg) { // accende o spengo un led se la temperatura è più bassa di quella di controllo
          digitalWrite(ledPintemp, HIGH); //accendo
          Serial.print("ON pin "); //scrivo in console
          Serial.println(ledPintemp); //scrivo in console
        }
        else {
          digitalWrite(ledPintemp, LOW); //spengo
          Serial.print("OFF pin "); //scrivo in console
          Serial.println(ledPintemp); //scrivo in console
        } 
 
        if (light < 150) { // accende o spengo un led se la c'è luce
          digitalWrite(ledPinlux, HIGH); // accendo
          Serial.print("ON pin "); //scrivo in console
          Serial.println(ledPinlux); //scrivo in console
        }
        else {
          digitalWrite(ledPinlux, LOW); //spengo
          Serial.print("OFF pin "); //scrivo in console
          Serial.println(ledPinlux); //scrivo in console
        } 
 
} // fine loop
