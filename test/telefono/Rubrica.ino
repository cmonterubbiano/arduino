//INSERITE I NUMERI DI TELEFONO DI CHI VOLETE

unsigned long nonna = 3333333333;
unsigned long zia = 3333333333;
unsigned long mamma = 3333333333;
unsigned long papa = 3333333333;


byte y_rub = 1; //è l'asse y dei  numeri di telefono
/*
con questa funzione posso chiamare numeri memorizzati all'interno di arduino
 se premo:
 - 5 chiamo il numero scelto
 - 2 vado su
 - 8 vado giù
 */
void rubrica_chiamata(void)
{
  lcd.clear();
  livello_batteria();
  
  if ( numero_digitato == 9 ) return;
  
  
  if (digitalRead(6) == 1) Chiamata();
  else if (numero_digitato == 2) y_rub = y_rub - 1;
  else if (numero_digitato == 8) y_rub = y_rub + 1;
  else y_rub = y_rub;

  lcd.setCursor(0, 0);
  lcd.print(" ADDRESS BOOK");
  lcd.setCursor(0, 1);
  //-------------------------------------LIMITI RUBRICA--------------------------------------------------
  if (y_rub >= 5) y_rub = 1;
  else if (y_rub == 0) y_rub = 4;
  else y_rub = y_rub;
  //-----------------------------------------------------------------------------------------------------
  switch (y_rub)
  {
    case 0:
      lcd.setCursor(0, 0);
      lcd.print(" ADDRESS BOOK");
      break;
    case 1:
      lcd.print("1-ZIA   ");
      numero_da_chiamare = zia;
      break;
    case 2:
      lcd.print("2-NONNA   ");
      numero_da_chiamare = nonna;
      break;
    case 3:
      lcd.print("3-MAMMA   ");
      numero_da_chiamare = mamma;
      break;
    case 4:
      lcd.print("4-PAPA   ");
      numero_da_chiamare = papa;
      break;
  }
  tastiera();
  delay(50);
}
void rubrica_messaggi(void)// QUESTA FUNZIONE RESTITUISCE IL VALORE
{
  lcd.clear();
  livello_batteria();
  if (digitalRead(6) == 1)
  {
    INVIO_SMS();//COL TASTO CHIAMATA INVIO IL MESSAGGIO
    for (int vv = 0; vv <= 100; vv++)
    {
      lcd.clear();
      lcd.print("    SENDING ");
      delay(40);
    }
    return;
  }
  else if (numero_digitato == 2) y_rub = y_rub - 1;
  else if (numero_digitato == 8) y_rub = y_rub + 1;
  else y_rub = y_rub;

  lcd.setCursor(0, 0);
  lcd.print(" ADDRESS BOOK");
  lcd.setCursor(0, 1);
  //-------------------------------------LIMITI RUBRICA--------------------------------------------------
  if (y_rub >= 5) y_rub = 1;
  else if (y_rub == 0) y_rub = 4;
  else y_rub = y_rub;
  //-----------------------------------------------------------------------------------------------------
  switch (y_rub)
  {
    case 0:
      lcd.setCursor(0, 0);
      lcd.print(" ADDRESS BOOK   ");
      break;
    case 1:
      lcd.print("1-ZIA   ");
      numero_da_chiamare = zia;
      break;
    case 2:
      lcd.print("2-NONNA   ");
      numero_da_chiamare = nonna;
      break;
    case 3:
      lcd.print("3-MAMMA   ");
      numero_da_chiamare = mamma;
      break;
    case 4:
      lcd.print("4-PAPA   ");
      numero_da_chiamare = papa;
      break;
  }
  tastiera();
  delay(50);
}



