void Menu_principale(void)
{
        lcd.setCursor(0, 0);
      lcd.print("  SMS  mode(4)");
      lcd.setCursor(0, 1);
      lcd.print("  CALL mode(6)");
      livello_batteria();
      tastiera();
  }
  void Risposta_chiamata (void)
{
        delay(100);
      mySerial.println("ATA");//con questo comando posso rispondere ad una chiamata
      lcd.clear();
      do
      {
        lcd.setCursor(0, 0);
        lcd.print("CONVERSATION.");
        delay(350);
        lcd.setCursor(0, 0);
        lcd.print("CONVERSATION..");
        delay(350);
        lcd.setCursor(0, 0);
        lcd.print("CONVERSATION...");
        delay(350);
        lcd.setCursor(0, 0);
        lcd.print("CONVERSATION....");
        lcd.clear();
      }
      while (digitalRead(6) == 0);//continua a scrivere CONVERSAZIONE finchè non ripremo il tasto 6 con il quale posso attaccare
      mySerial.println("ATH");//comando per rispondere
      uscita();
        }
void Decisione_scrittura(void)
{
       if (numero_digitato == 9) return; //PREMENDO IL TASTO 9 SI RITORNA NEL MENU INIZIALE
      else {
        lcd.setCursor(0, 0);
        lcd.print("Address book=4");
        lcd.setCursor(0, 1);
        lcd.print("Enter number=6");
        livello_batteria();
        tastiera();
      } 
  }
  void Digitazione_numero(void)
  {
                  lcd.setCursor(0, 0);
          lcd.print("Write number");
          tastiera();
          inserimento_numero();
    }
