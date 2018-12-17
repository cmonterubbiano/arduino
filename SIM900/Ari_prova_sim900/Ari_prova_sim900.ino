/*Note: this code is a demo for how to using GPRS Shield to send SMS message and dial a voice call.

The microcontrollers Digital Pin 7 and hence allow unhindered communication with GPRS Shield using SoftSerial Library. 
IDE: Arduino 1.0 or later */
 
 
#include <SoftwareSerial.h>
#include <String.h>
unsigned char buffer[64]; // buffer per i dati scambiati sulla seriale
int count=0;              // contatore dati nel buffer
  
SoftwareSerial mySerial(7,8);
 
void setup()
{
  mySerial.begin(9600);               // the GPRS baud rate   
  Serial.begin(9600);    // the GPRS baud rate 
  delay(500);
  Serial.println("Digitare 't' per inviare un messaggio");
  Serial.println("Digitare 'r' per leggere un messaggio mai letto");
  Serial.println("Digitare 'd' per chiamare un numero");
}
 
void loop()
{
  //after start up the program, you can using terminal to connect the serial of gprs shield,
  //if you input 't' in the terminal, the program will execute SendTextMessage(), it will show how to send a sms message,
  //if input 'd' in the terminal, it will execute DialVoiceCall(), etc.
  if (Serial.available())
  {
    switch(Serial.read())
   {
     case 't':
           Serial.println("Send t");
       SendTextMessage();
       break;
     case 'd':
           Serial.println("Send d");
       DialVoiceCall();
       break;
     case 'r':
           Serial.println("Leggi r");
       ReadTextMessage();
       break;   
   } 
  if (mySerial.available())
    Serial.write(mySerial.read());
  }
}
///SendTextMessage()
///this function is to send a sms message
void SendTextMessage()
{
  mySerial.print("AT+CMGF=1\r");    //Because we want to send the SMS in text mode
  delay(100);
  mySerial.println("AT + CMGS = \"337634273\"");//send sms message, be careful need to add a country code before the cellphone number
  delay(100);
  mySerial.println("How are you ?");//the content of the message
  delay(100);
  mySerial.println((char)26);//the ASCII code of the ctrl+z is 26
  delay(100);
  mySerial.println();
}
 
///DialVoiceCall
///this function is to dial a voice call
void DialVoiceCall()
{
  mySerial.println("ATD337634273;");//dial the number
  delay(100);
  mySerial.println();
}
 
void ShowSerialData()
{
  while(mySerial.available()!=0)
    Serial.write(mySerial.read());
}

///ReadTextMessage()
///this function is to send a sms message
void ReadTextMessage()
{
  mySerial.print("AT+CMGL=\"ALL\"\r");    //Because we want to send the SMS in text mode
    mySerial.println();
  delay(100);
ShowSerialData();
}
void clearBufferArray() // Funzione di supporto per svuotare il buffer
{
  for (int i=0; i<count;i++)
      buffer[i]=NULL;                  
}
