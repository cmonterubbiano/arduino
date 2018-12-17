
/* Per limitare questi errori possiamo adottare due tecniche,
la prima è quella di usare il pin AREF dell’Arduino UNO per
dare al convertitore analogico digitale dell’ATmega328 un
riferimento di tensione più basso (di default è 5Vdc),
l’altra tecnica è quella di effettuare diverse letture ed
eseguire una media dei valori letti.
Nello schema seguente viene indicato come collegare il pin AREF
al pin power di 3.3Vdc:
*/
/* TMP36
guardando frontalmente il TMP36 troviamo sul lato sinistro
il pin di alimentazione, sul pin centrale il segnale in uscita
e sul pin destro il collegamento a massa.
*/
/*
la formula che converte il valore acquisito in gradi centigradi è la seguente
°C = ((valoreADC * PrecisioneADC) – TensioneZeroGradi) / stepGradoTensione
dove
°C = valore della temperatura in gradi centigradi
valoreADC = valore della conversione analogico digitale restituito
            da analogRead
PrecisioneADC = questo valore è ottenuto dividendo la tensione
                di riferimento dell’ADC (default 5Vdc) e il numero
                massimo restituito dalla conversione
                (1024). (5Vdc /1024 = 0.00488)
TensioneZeroGradi = indica la tensione di uscita dal sensore quando
                    rileva una temperatura di 0°C
stepGradoTensione = indica la variazione di tensione per ogni variazione
                    di grado (0.01 = 10 mV)
*/

int val_Adc = 0;
float  lettura =0;
float  minima =0;
float  massima =0;
float temp = 0;
 
void setup()
{
  //init seriale
  Serial.begin(9600);
 
  //utilizzando l'istruzione analogReference
  //indico al convertitore AD che deve impiegare
  //la tensione presente sul pin AREF come
  //valore di riferimento per la conversione
  analogReference(EXTERNAL);
}
 
void loop()
{
  //ritardo di mezzo secondo
  delay(500);
 
  //init variabile
  val_Adc = 0;
  minima =1000;
  massima =-1000;
 
  //eseguo un ciclo
  for(byte Ciclo = 0; Ciclo <102; Ciclo++)
  {
    //questo ritardo serve per dare il tempo
    //all' ADC di eseguire correttamente
    //la prossima acquisizione
    delay(10);
    lettura =analogRead(0);
    if (!Ciclo)
    {
      minima =lettura;
      continue;
    }
    if (Ciclo ==1)
    {
      massima =lettura;
      continue;
    }
    if (lettura <minima)
    {
      val_Adc +=minima;
      minima =lettura;
      continue;
    }
    if (lettura >massima)
    {
      val_Adc +=massima;
      massima =lettura;
      continue;
    }
    val_Adc +=lettura;
  }
 
  //eseguo la media dei 100 valori letti
  val_Adc /= 100;
 
  //calcolo la temperatura in °C
  //questo calcolo vale x 3,3 v
  temp = ((val_Adc * 0.0032) - 0.5) / 0.01;
  minima = ((minima * 0.0032) - 0.5) / 0.01;
  massima = ((massima * 0.0032) - 0.5) / 0.01;
  // quello sotto meno preciso x 5V
  //temp = ((val_Adc * 0.00488) - 0.5) / 0.01;
  //minima = ((minima * 0.00488) - 0.5) / 0.01;
  //massima = ((massima * 0.00488) - 0.5) / 0.01;
 
  //invio i dati al computer
  Serial.print(temp);
  Serial.print(" - minima ");
  Serial.print(minima);
  Serial.print(" - massima ");
  Serial.println(massima);
}
