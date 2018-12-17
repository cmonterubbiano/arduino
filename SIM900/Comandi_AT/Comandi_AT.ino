/* -----------------------------------------------------------------------------------
 Consente di inviare i comandi AT direttamente dal Serial Monitor
 ---------------------------------------------------------------------------------- */
#include <SoftwareSerial.h>
SoftwareSerial SIM900(7, 8);
char smsbuffer[160];
char incoming_char=0;

// legge una stringa dal Serial Monitor e traduce \n e \r in 
boolean SerialRead(char s[])
{
  int i=0;
  boolean EscCode=false;
  boolean CtrlCode=false;
  if (Serial.available() > 0)
  {            
    while (Serial.available() > 0) 
    {
      s[i]=Serial.read();
      if (EscCode)
      {
          // sostituisco le stringhe "\n" e "\r" con le
          // i caratteri '\r' e '\n'
          if (s[i]=='r')
             s[i]='\r'; // carattere CR
          else if (s[i]=='n')
             s[i]='\n'; // carattere LF
          else
          {
            i++;
            s[i]=s[i-1];
            s[i-1]='\\';
          }
          i++;
          EscCode=false;
      }
      else if (CtrlCode)
      {
        if (s[i]=='z')
        {
           s[i]=(char)26; // ctrl+Z
      Serial.print("^Z\n");  
      }else
        {
            i++;
            s[i]=s[i-1];
            s[i-1]='^';
        }
        i++;
        CtrlCode=false;
      }
      else if (s[i]=='\\')
         EscCode=true;
      else if (s[i]=='^')
         CtrlCode=true;
      else
         i++;      
      delay(10);
    }
  }
  s[i]='\0';
  return (i!=0);
}
void setup()
{
  Serial.begin(9600); // Ricordarsi di porre la connessione sulla finestra del Serial
                       // monitor a 19200
  SIM900.begin(9600); // Setta la velocità di comunicazione sulla seriale del cellulare
  Serial.print("Terminale Comandi AT:\n");
}

void loop()
{
  char position;
  if(SIM900.available() >0)
  {
    incoming_char=SIM900.read(); // Legge il carattere sulla porta seriale del cellulare
    Serial.print(incoming_char); // e lo mostra sul serial monitor
  }
  position=SerialRead(smsbuffer); 
  if (position)
     SIM900.print(smsbuffer);
}
