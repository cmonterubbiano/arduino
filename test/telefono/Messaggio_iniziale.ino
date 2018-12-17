
void schermata_avvio(void)
{
  for ( byte zz = 0 ; zz < 30 ; zz++ )
  {
    lcd.setCursor(3, 0);
    lcd.print("ARDUFONINO");
    lcd.setCursor(0, 1);
    //lcd.print("the future phone");
    //lcd.print  ("created by D.A");
    lcd.print  ("copyright by D.A");
    delay(200);
  }
  lcd.clear();  
}

