void INVIO_SMS()
{



  mySerial.print("AT+CMGF=1\r");    //SI può scegliere tra 0 = PDU MODE ed 1 = TEXT MODE
  delay(100);
  String numero = String(numero_da_chiamare); //converto il numero da chiamare in stringa
  mySerial.println("AT + CMGS = \"+39" + numero + "\"");
  delay(100);//   dovrebbe tirare fuori questo AAT + CMGS = "+393286773254" GIUSTO

  if (y_rub_SMS == 0)   lcd.print("     Testo   "); // PRENDE IL DATO y_rub_SMS e lo usa per scegliere la frase
  else if (y_rub_SMS == 1)    mySerial.println("CIAO!");
  else if (y_rub_SMS == 2)    mySerial.println("COME STAI? ");
  else if (y_rub_SMS == 3)    mySerial.println("A CHE ORA VIENI? ");
  else if (y_rub_SMS == 4)    mySerial.println("CHI SEI?");
  else if (y_rub_SMS == 5)    mySerial.println("TRA UN PO");
  else if (y_rub_SMS == 6)    mySerial.println("ECCOMI");
  else if (y_rub_SMS == 7)    mySerial.println("AUGURI!");
  else if (y_rub_SMS == 8)    mySerial.println("TI CHIAMO IO");
  else if (y_rub_SMS == 9)    mySerial.println("DOVE CI VEDIAMO?");
  else if (y_rub_SMS == 10)   mySerial.println("OK");

  else return;

  delay(100);
  mySerial.println((char)26);//the ASCII code of the ctrl+z is 26
  delay(100);
  mySerial.println();
}

void  testo_SMS(void)// RESTITUISCE IL VALORE DELLA VARIABLE y_rub_SMS ALLA FUNZIONE DI INVIO MESSAGGIO PER SCEGLIERE LA FRASE
{


  lcd.clear();
  livello_batteria();

  if (numero_digitato == 2) y_rub_SMS = y_rub_SMS - 1;
  else if (numero_digitato == 8) y_rub_SMS = y_rub_SMS + 1;
  else y_rub_SMS = y_rub_SMS;

  lcd.setCursor(0, 0);
  lcd.print("  Choose Text:");
  lcd.setCursor(0, 1);

  //-------------------------------------LIMITI RUBRICA--------------------------------------------------
  if (y_rub_SMS >= 11) y_rub_SMS = 1;
  else if (y_rub_SMS == 0) y_rub_SMS = 10;
  else y_rub_SMS = y_rub_SMS;
  //-----------------------------------------------------------------------------------------------------


  switch (y_rub_SMS)
  {
    case 0:
      lcd.setCursor(0, 0);
      lcd.print("     Text   ");
      break;
    case 1:
      lcd.print("CIAO!                         ");
      break;
    case 2:
      lcd.print("COME STAI?                      ");
      break;
    case 3:
      lcd.print("A CHE ORA VIENI?          ");
      break;
    case 4:
      lcd.print("CHI SEI?                      ");
      break;
    case 5:
      lcd.print("TRA UN PO                      ");
      break;
    case 6:
      lcd.print("ECCOMI!                      ");
      break;
    case 7:
      lcd.print("AUGURI!                      ");
      break;
    case 8:
      lcd.print("TI CHIAMO IO                     ");
      break;
    case 9:
      lcd.print("DOVE CI VEDIAMO?                      ");
      break;
    case 10:
      lcd.print("OK                      ");
      break;
  }
  tastiera();
  delay(50);
}


void lettura_SMS(void)
{
  if (mySerial.available() > 0)
  {
    testo_messaggio = mySerial.read();
    Serial.print(testo_messaggio); 
    lcd.print(testo_messaggio);
  }
}




