unsigned int frequenza_scritta = 140;


void Chiamata()
{
  lcd.clear();
  String numero = String(numero_da_chiamare); //converto il numero da chiamare in stringa
  mySerial.println("ATD + +39" + numero + ";"); //Effettuo chiamata
  delay(100);
  mySerial.println();
  lcd.clear();
  do
  { 
    lcd.setCursor(0, 1);
    lcd.print("   ");//spazio per il numero da inserire
    lcd.print(numero);
    lcd.setCursor(0, 0);
    lcd.print("CALLING");
    delay(frequenza_scritta);
    lcd.setCursor(0, 0);
    lcd.print("CALLING.");
    delay(frequenza_scritta);
    lcd.setCursor(0, 0);
    lcd.print("CALLING..");
    delay(frequenza_scritta);
    lcd.setCursor(0, 0);
    lcd.print("CALLING...");
    delay(frequenza_scritta);
    lcd.setCursor(0, 0);
    lcd.print("CALLING....");
    delay(frequenza_scritta);
    lcd.setCursor(0, 0);
    lcd.print("CALLING.....");
    delay(frequenza_scritta);
    lcd.setCursor(0, 0);
    lcd.print("CALLING......");
    delay(frequenza_scritta);
    lcd.setCursor(0, 0);
    lcd.print("CALLING.......");
    delay(frequenza_scritta);
    lcd.setCursor(0, 0);
    lcd.print("CALLING........");
    delay(frequenza_scritta);
        lcd.setCursor(0, 0);
    lcd.print("CALLING........");
    delay(frequenza_scritta);
    lcd.clear();
  }
  while (digitalRead(6) == 0);//continua a scrivere CONVERSAZIONE finchè non ripremo il tasto 6 con il quale posso attaccare
  mySerial.println("ATH");//comando per rispondere
   uscita();
}

