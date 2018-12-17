int tastiera(void)
{
  do 
  {  
    lettura_analogica_tastiera = analogRead(A5);
    delay(10);
  }
  while ((lettura_analogica_tastiera < 50)&&(digitalRead(6)==0));//questa riga mi permette di rispondere alla chiamata perchè oltre che a leggere la tastiera leggo anche il tasto 6
  delay(100);//antirimbalzo
  numero_digitato = map(lettura_analogica_tastiera, 0, 1023 , 0 , 9);
  return numero_digitato;
}
