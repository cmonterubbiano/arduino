/*
  LiquidCrystal Library - Hello World
 
 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the 
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.
 
 This sketch prints "Hello World!" to the LCD
 and shows the time.
 
  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 
 VSS  GROUND
 VDD  +5
 VO   centrale potenziometro da 10 K estremi + 5 e massa (contrasto)
 RS   pin 12
 RW   GROUND
 E    pin 11
 D4   pin 5
 D5   pin 4
 D6   pin 3
 D7   pin 2
 A    centrale potenziometro da 10 K estremi + 5 e massa (illuminazione)
 K    GROUND
 
 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int  attesa =40;    // 40 condizioni normali se attesa prima chiamata e' 35 secondi
int  secondi =0;
int  tempomax =180;  // dovrebbe essere cirva 300 ossia 5 minuti
char mess[17];
int  spegni =10;
int  citofono =8;
int  pulsante =9;
int  buttonState = 0;         // variable for reading the pushbutton status
int  contatore =0;

void setup() 
{
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  sprintf(mess, "%-16.16s", "     ATTESA");
  lcd.print(mess);
  Serial.begin(9600);
  pinMode(citofono, OUTPUT);
  pinMode(spegni, OUTPUT);
  pinMode(pulsante, INPUT);
  digitalWrite(citofono, LOW);
  digitalWrite(spegni, LOW);
}

void loop() 
{
  int  tempo;
  for (;attesa ; attesa--, tempomax--, secondi++)
  {
    lcd.setCursor(0, 1);
    if (attesa >5)
    {
      sprintf(mess, "     - %2d -", (attesa -5));
      lcd.print(mess);
    }
    else
    {
      lcd.setCursor(0,0);
      sprintf(mess, "%-16.16s", "    CHIAMATA");
      lcd.print(mess);
      lcd.setCursor(0,1);
      sprintf(mess, "%-13.13s %2d", " IN CORSO", attesa);
      lcd.print(mess);
      switch (attesa)
      {
        case 5:
          digitalWrite(citofono, HIGH);
          Serial.println("CHIAMO");
          break;
        case 4:
          digitalWrite(citofono, LOW);
          Serial.println("NON CHIAMO +");
          break;
        default :
          break;
      }
    }
    delay(1000);
  }
  buttonState = digitalRead(pulsante);
  if (buttonState == HIGH)
  {         
    digitalWrite(citofono, HIGH);
    sprintf(mess, "pulsante %d-", secondi);  
    Serial.println(mess);
    delay(1000);      // se il pulsante e' pigiato aspetto un secondo
    contatore +=10;
  } 
  else
  {
    digitalWrite(citofono, LOW);
    delay(100);    // se il pulsante non e' pigiato aspetto un decimo di secondo
    contatore +=1; 
  }
  secondi =(contatore /10);
    
  Serial.print("contatore ");
  Serial.print(contatore);
  Serial.print("-secondi ");
  Serial.println(secondi);
  lcd.setCursor(0,0);
  sprintf(mess, "%-16.16s", "     CITOFONO");
  lcd.print(mess);
  lcd.setCursor(0,1);
  tempo =tempomax -secondi;
  sprintf(mess, "%d:%02d %s", (tempo/60), (tempo %60), "in Funzione ");
  lcd.print(mess);
  if (tempo <=0)
  {
    Serial.print("TOLGO ALIMENTAZIONE");
    digitalWrite(spegni, HIGH);
    // se l'alimentazione non avviene tramite i 12 volt ed il marchingegno dell'auto ritenuta
    // il programma prosegue, in condizioni di regime il citofono/arduino si spengono
    // e le istruzioni successive non vengono eseguite, per cui le istruzioni successive
    // hanno senso solo con alimentazione da USB o lo spegnimento non e' avvenuto
    lcd.setCursor(0,0);
    sprintf(mess, "%-16.16s", "     ATTESA");
    lcd.print(mess);
    lcd.setCursor(0,1);
    sprintf(mess, "%-16.16s", "");
    lcd.print(mess);                                                                                                                                                                        ;  
    secondi =0;
    delay(1000);
    digitalWrite(spegni, LOW);
    contatore =0;
  }
}

