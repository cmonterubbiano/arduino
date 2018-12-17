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
int  attesa =40;
int  secondi =0;
int  tempomax =62;
char  mess[17];

void setup() 
{
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  sprintf(mess, "%-16.16s", "     ATTESA");
  lcd.print(mess);
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(10, OUTPUT);
  digitalWrite(13, LOW);
  digitalWrite(10, LOW);
}

void loop() 
{
  for (;attesa ; attesa--, secondi++)
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
      sprintf(mess, "%-16.16s", " CHIAMATA");
      lcd.print(mess);
      lcd.setCursor(0,1);
      sprintf(mess, "%-13.13s %2d", " IN CORSO", attesa);
      lcd.print(mess);
      switch (attesa)
      {
        case 5:
          digitalWrite(13, HIGH);
          Serial.print("ACCENDO");
          break;
        case 4:
          digitalWrite(13, LOW);
          Serial.print("SPENGO");
          break;
        default :
          break;
      }
    }
    delay(1000);
//    sprintf(mess, "%3d%c", secondi, '\n');
//    Serial.print(mess);
  }
  secondi++;
  lcd.setCursor(0,0);
  sprintf(mess, "%-16.16s", "     CITOFONO");
  lcd.print(mess);
  lcd.setCursor(0,1);
  sprintf(mess, "%d:%02d %s", (secondi/60), (secondi %60), "in Funzione ");
  lcd.print(mess);
  delay(1000);
  if (secondi >=tempomax)
  {
    Serial.print("TOLGO ALIMENTAZIONE");
   // digitalWrite(12, HIGH);
    lcd.setCursor(0,0);
    sprintf(mess, "%-16.16s", "     ATTESA");
    lcd.print(mess);
    lcd.setCursor(0,1);
    sprintf(mess, "%-16.16s", "");
    lcd.print(mess);
    attesa =40;
    secondi =0;
    delay(1000);
    digitalWrite(12, LOW);
  }
}

