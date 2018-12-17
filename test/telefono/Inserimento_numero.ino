void inserimento_numero(void)
{
  lcd.setCursor(10, 1);
  lcd.print("-");
  lcd.setCursor(asse_x, 1);
  asse_x++;//mi permette di passare da una colonna all'altra
  numeri_inseriti++;

  if (numeri_inseriti >= 12) //10
  {
    lcd.clear();//se il numero supera le 10 cifre cancella tutto
    asse_x = 0;
    numero_da_chiamare = 0;
    numeri_inseriti = 0;
    potenza_posizione_numero = 1000000000;
  }
  else
  {
    lcd.print(numero_digitato);
    numero_da_chiamare = numero_da_chiamare + ((numero_digitato) * potenza_posizione_numero);
    potenza_posizione_numero = potenza_posizione_numero / 10;
    lcd.setCursor(13, 0);
    lcd.print(numeri_inseriti);//scrive in alto a destra il numero di numeri inseriti
    delay(50);
  }
}

