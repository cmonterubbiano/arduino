/*
  ARDUFONINO SOFTWARE
 
Trasforma il tuo arduino in un vero e proprio cellulare capace di fare e ricevere chiamate e di inviare SMS.
 
Tutti i schemi e le informazioni si troveranno sul mio sito 

www.davidealoisi.it
 
 creato il 5  May 2014
 modificato il ............

 by Davide Aloisi 

 */

#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//-------------------------------------------------VARIABILI---------------------------------------------------
char testo_messaggio = 0;
int numero_digitato = 3;
byte numeri_inseriti = 0;
byte asse_x;
byte y_rub_SMS = 1;//asse Y della rubrica degli SMS
unsigned long numero_da_chiamare; //ho messo unsigned float perchè è la variabile più grande che c'è che può valere fino a  4,294,967,295
unsigned long potenza_posizione_numero = 1000000000;
int lettura_analogica_tastiera = 0;
byte m = 9;
int stato_batteria=0;
int batteria=0;
//--------------------------------------------------------------------------------------------------------------

SoftwareSerial mySerial(7, 8); // Creazione seriale

//-------------------------------------------------PROTOTIPI---------------------------------------------------
void lettura_SMS(void);
void accensione_GSM(void);
void livello_batteria(void);
void rubrica_chiamata(void);
void rubrica_messaggi(void);
//-------------------------------------------------------------------------------------------------------------

void setup()
{
  mySerial.begin(19200);               // Velocità seriale GSM
  Serial.begin(57600);                 // Velocità seriale Arduino
  lcd.begin(16, 2);
  accensione_GSM();
  pinMode(6, INPUT);
  //----------------------Parte di ricezione deli sms------------------------------------------
  mySerial.print("AT+CMGF=1\r");
  delay(100);
  mySerial.print("AT+CNMI=2,2,0,0,0\r");
  delay(100);
  //-----------------------------------------------------------------------------------------
  mySerial.println("AT+CLVL = 99"); //il volume dell'uscita audio va da 0 a 100
  mySerial.println(" AT+CRSL = 3 "); //Metto il volume della suoneria a 3 il volume va da 0 a 4
  delay(500);
  schermata_avvio();
}


void loop()
{
  
 lettura_SMS();//ANDREBBE LETTA IN INTERRUPT
 
 {
  lcd.clear();
  do
  {
    if (digitalRead(6) == 1) Risposta_chiamata();
    else Menu_principale();
  }
  while ((numero_digitato != 6) && (numero_digitato != 4)); //finchè non si premono il tasto 4 o 6 il menù iniziale non si leva

  delay(500);//evita false letture

  if (numero_digitato == 6)
{
  lcd.clear();
    do Decisione_scrittura();
    while ((numero_digitato != 4) && (numero_digitato != 6) && (numero_digitato != 9) ); //finchè non si premono il tasto 4 o 6 il menù iniziale non si leva
    delay(500);//questo delay serve a non far apparire i numeri delle false letture iniziali

    if (numero_digitato == 6)
    {
      lcd.clear();
      do
      {
        //-----------------------------------------------------------DIGITAZIONE NUMERO---------------------------------------------
        if (digitalRead(6) == 1)
        {
          lcd.clear();
          Chiamata();
          return;
        }
        else Digitazione_numero();
 }
      while (m == 9);
    }
    else
    {
      if (numero_digitato == 4)
      {
        lcd.clear();
        do
        {
          livello_batteria();
          rubrica_chiamata();
        }
        while ((m == 9) && (numero_digitato != 9));
      }
    }
  }
//------------------------------------------------------------MESSAGGI----------------------------------------------------------
  if (numero_digitato == 4)
{
  delay(100);
  
    do testo_SMS(); // scelta testo da inviare
    while ((m == 9) && (numero_digitato != 9) && (numero_digitato != 5));

    lcd.clear();
    
    do Decisione_scrittura();
    while ((numero_digitato != 4) && (numero_digitato != 6)&& (numero_digitato != 9)); //Decisione del numero a cui inviare

    delay(400);

    if (numero_digitato == 4) //Invio il messaggio ad un numero della rubrica
    {
      do rubrica_messaggi();
      while ((m == 9) && (numero_digitato != 9));
      return;
    }
    else if (numero_digitato == 6) //Invio il messaggio ad un numero che scrivo
    {
      lcd.clear();
      
      do Digitazione_numero();
      while ((m == 9) && (digitalRead(6) == 0));

      INVIO_SMS();//COL TASTO CHIAMATA INVIO IL MESSAGGIO
      for (int vv = 0; vv <= 100; vv++)
      {
        lcd.clear();
        lcd.print("SENDING  ");
        delay(20);
      }
      return;
    }
  }
}

}


void accensione_GSM(void)
{
  pinMode(9, OUTPUT);
  delay(200);
  digitalWrite(9, LOW);
  delay(500);
  digitalWrite(9, HIGH);
  delay(500);
  digitalWrite(9, LOW);
  delay(500);
}

void uscita(void) // la utilizzo quando devo riattaccare ad una chiamata 
{
  delay(600);//con il delay evito di rileggere il tasto 6 quando attacco
  return;//ritorno al menu iniziale 
}






