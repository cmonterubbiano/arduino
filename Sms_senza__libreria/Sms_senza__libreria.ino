#define led 13

#include <SoftwareSerial.h>

SoftwareSerial sim900(7,8);
char istruzioni_sim[100];

void setup(){
 Serial.begin(9600);
 sim900.begin(9600);
 pinMode(led, OUTPUT); 
 delay(500);
}

void loop(){
 leggi_da_sim900(); 
}

void leggi_da_sim900(){  
 boolean ricevuto_sms = false;
 int i = 0;
 int numero_sms = 0;

 while(sim900.available() >0){
   char carattere = sim900.read();
   istruzioni_sim[i] = carattere;

   if(strcmp(istruzioni_sim, "\r\n+CMTI: \"SM\",")==0){   //vuol dire che ho ricevuto dalla shield la funzione di un nuovo sms
     carattere = sim900.read() -'0';                      //il carattere dopo la virgola è un numero; rappresenta l'ultimo sms
     numero_sms = carattere;  
     istruzioni_sim[i+1] = carattere;                      //salvo ugualmente il numero nel vettore istruzioni_sim
     ricevuto_sms = true;                                  //mi servirà per leggere l'sms solo quando lo ricevo
   }
   i++;
   delay(5);
 }

 if(ricevuto_sms == true){                                 //a questo punto vuol dire che ho ricevuto un sms
   while(i>0){
     istruzioni_sim[i] = NULL;                             //posso svuotare il vettore
     i--; 
   }
   leggi_sms(numero_sms);                                  //passo alla funzione leggi_sms il valore dell'ultimo sms da leggere
   i=0;
   ricevuto_sms = false;
 }
}


void leggi_sms(int ultimo_sms){
 String sms;                                            //ho utilizzato una variabile string perchè non sapevo quanto dovesse essere lungo un sms
 int inizio_sms = 0;
 int count = 0;
 sim900.print("AT+CMGR=");                              //invio alla shield, la richiesta di lettura di sms
 sim900.println(ultimo_sms);                            //attenzione al println
 delay(200);

 while(sim900.available() >0){
   char carattere = sim900.read();  //inizio a leggere l'sms
   if(carattere == '\"'){
     inizio_sms = count;                                //questa funzione si occupa di vedere dove inizia il testo dell'sms visto che la strnga ricevuta è del tipo ABC"\r\nTESTOSMS\r\n\r\n\r\nOK
   }
   sms += carattere;
   count++;
   delay(5);
 }

 sms.toLowerCase();                                                 //rendo tutto minuscolo
 
 //Serial.print(sms.substring((inizio_sms+3), (count-8)));         //questo perchè la stringa è composta come segue: ABC"\r\nBLA\r\n\r\n\r\nOK, comunque se abiliti questo Serial.print viene visualizzato il corpo dell'sms
 if(sms.substring((inizio_sms+3), (count-8)) == "accendi"){        //la parola accendi, è la mia parola chiave, può essere comunque cambiata per eseuire diverse azioni
   digitalWrite(led, HIGH);
 }
 else if(sms.substring((inizio_sms+3), (count-8)) == "spegni"){
   digitalWrite(led, LOW);
 }
  sms="";
 sim900.print("AT+CMGD=");                                          //invio alla shield la richiesta di eliminare l'ultimo_sms
 sim900.println(ultimo_sms);
 delay(100);
}

