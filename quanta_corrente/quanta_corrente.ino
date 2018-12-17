//** ********************************************************************* **//
//**                                                                       **//
//** AlertHighPower                                                        **//
//**                                                                       **//
//** Rilevatore di troppa corrente  per sensore SCT-013-020 o SCT-013-030  **//
//**                                                                       **//
//** ********************************************************************* **//
//** Lo scopo del programma Ã¨ quello fornire  un valore, contenuto in  una **//
//** variabile, da usare  per accendere  un relÃ¨ o emettere segnali sonori **// 
//** quando in una casa si accendono contemporaneamente due grossi carichi **// 
//** elettrici (forno,  phon,  lavatrici, lavastoviglie  ecc.; in generale **// 
//** quelli superiori a 1500W).                                            **//
//**                                                                       **//
//** E' un programma utile al fine di evitare  il  distacco  del contatore **// 
//** elettrico per troppa potenza assorbita.                               **//
//**	                                                                   **// 
//** Tuttavia il programma funziona  a condizione  che lâ€™ assorbimento del **// 
//** maggiore carico elettrico, acceso insieme  a tutti  gli altri carichi **// 
//** inferiori (tv, lampadine, computer ecc.) non sia tale da far scattare **// 
//** il contatore.  Una  condizione  rispettata  in  quasi  tutte  le case **// 
//** medio-piccole.                                                        **//
//**                                                                       **//
//** Per la prima impostazione:                                            **//  
//** Lanciate il programma. Rilevate il valore (max) dato dallâ€™assorbimen- **//
//** to di due grossi carichi. Rilevate il valore (min) dato dal piÃ¹ gros- **//
//** so carico che avete acceso insieme con tutti i carichi minori.  Assu- **//
//** mete  la  media  tra  questi  due  valori  come valore  di soglia ed  **//
//** impostatelo nellâ€™ ultimo  IF del programma (...if  somma >110â€¦ )  al  **//
//** posto del numero 110.                                                 **//  
//**                                                                       **//
//**                                                                       **//
//** Maggiori informazioni in myelectronichome.altervista.org.             **//
//**                                                                       **//
//** Scritto da Corrado Bevilacqua nei ritagli di tempo.                   **//
//** Iniziato il 23/09/2013. Software e debug completato il 31/12/2013.    **//
//** Versione che non memorizza in un vettore i  singoli valori dell' onda **//
//** per risparmiare memoria.                                              **//  
//** Lavora sulla media  delle ultime 5 letture  per contenere la presenza **//
//** di impulsi spuri "spike" sull'onda sinosuidale rilevata.              **//
//**                                                                       **//
//** Software liberamente copiabile a patto morale di citare l'autore nei  **//
//** commenti del vostro programma.                                        **//
//** ********************************************************************* **//
//**  Written by Corrado Bevilacqua (C) - myelectronichome.altervista.org  **// 
//** ********************************************************************* **//


//** ********************************************************************* **// 
//** Variabili per il sensore di corrente                                  **//
//** ********************************************************************* **//
int valmedio[5];
int analogPin = 1; //pin da collegare al rilevatore di corrente SCT 013 020
int zerovolt=0;
long Quantacorrenteoldtempo=0; //per evitare che controlli la corrente di continuo


void setup()  
     {
     Serial.begin(19200);     
     Serial.println("Sketch: AlertHighPower");
     delay(500);

     //** **************************************************************** **//
     //** Per il sensore di corrente - Rileva lo zerovolt                  **//
     //** **************************************************************** **//
     ZeroVolt();                                                      //** **//
     //** **************************************************************** **//
     }  // end setup


void loop() 
     {
     //controlla la corrente
     QuantaCorrente();
     }	//end loop 


void ZeroVolt()                     
     {   
	 //** **************************************************************** **// 
     //** Determino il valore da 2,5V a cui Ã¨ collegato il sensore         **//
     //** SCT 013-020. Lo calcolo ogni volta all'accensione dell'Arduino   **//
     //** per evitare la deriva dei componenti.                            **//                   **
     //** **************************************************************** **//
     
     int max2 =0;
     int min2 =9999;
     
     valmedio[0]=analogRead(analogPin);
     valmedio[1]=analogRead(analogPin);
     valmedio[2]=analogRead(analogPin);
     valmedio[3]=analogRead(analogPin);
     valmedio[4]=analogRead(analogPin);

     int punta=0;
     int mediato=0;       
   
     long oldtime1;
     oldtime1=millis();

     while (millis()-oldtime1<=85)
        {
        valmedio[punta]=analogRead(analogPin);
        if (punta<4) 
           {punta=punta+1;}
        else
           {punta=0;}
        
        mediato=(valmedio[0]+valmedio[1]+valmedio[2]+valmedio[3]+valmedio[4])/5;
   
        if (max2<mediato) 
           {max2=mediato;}
        if (min2>mediato) 
           {min2=mediato;}
         
        }
 

     zerovolt = (max2+min2)/2;  
     
     Serial.print("zerovolt = " );
     Serial.println(zerovolt);
       
     }	//end zerovolt



int QuantaCorrente()                     
     {
     if (abs(millis()-Quantacorrenteoldtempo) >= 10000)   //entra nel ciclo solo ogni dieci secondi   
        {

        long somma=0;
        long conta=0;
        int punta=0;
        int mediato=0;  
        int i=0;
        int PerZeroOk=0;  
        int tante = 10;             //indica quante mezze onde leggere. Con 10 semionde (5 periodi) ci mette 100ms
        
        //leggi valori per costruire la prima media    
        valmedio[0]=analogRead(analogPin);
        valmedio[1]=analogRead(analogPin);
        valmedio[2]=analogRead(analogPin);
        valmedio[3]=analogRead(analogPin);
        valmedio[4]=analogRead(analogPin);

        mediato=(valmedio[0]+valmedio[1]+valmedio[2]+valmedio[3]+valmedio[4])/5;
      
        boolean onda=0;
        if (mediato-zerovolt>0 )    //Se maggiore di uno allora per onda metto 1 altrimenti zero, Dice se l'onda Ã¨ sopra o sotto lo zero
           {onda=1;}

        long TempoStart;             //variabile per calcolare quanto tempo ci mette
        TempoStart=millis();

        while (PerZeroOk<=tante && abs(millis()-TempoStart) < (tante+4)*20)
           {
           //Mediamo i valori letti per contenere gli spikes    
           valmedio[punta]=analogRead(analogPin); 
           if (punta<4) 
              {punta=punta+1;}
           else
              {punta=0;}
           mediato=(valmedio[0]+valmedio[1]+valmedio[2]+valmedio[3]+valmedio[4])/5;
           mediato=mediato-zerovolt;
         
         
           // L'IF seguente Rileva il passaggio per zero e conseguentemente riesce a contare le semionde.
           // Tuttavia per bassi assorbimenti di corrente non riesce a contarle bene e quindi fornisce un 
		   // valore piÃ¹ basso di quello dovuto. E' un problema che  si presenta solo per  basse correnti  
		   // e quindi non inficia  il funzionamento del programma  perchÃ¨  lavora sulla fascia dei forti 
		   // assorbimenti di corrente. 

           if ((onda==1 && mediato <0) || (onda==0 && mediato >0))
              {
              PerZeroOk=PerZeroOk+1;
              onda=onda^B1;   //Cambia lo stato da 0 a 1 e viceversa
              }
                     
           //In PerZeroOK=0 siamo in un punto a caso della prima semionda per cui ignoriamone i valori. Dobbiamo prendere sempre semionde intere. 
           if (PerZeroOk>=1)
              {
              somma=somma+abs(mediato);
              conta=conta+1;  
              }
           } 

           TempoStart=millis()-TempoStart;   
           somma=somma/conta;       
                               							   							   
           Serial.print("Indicatore=");
           Serial.print(somma);
           Serial.print(" TempoElaborazione=");
           Serial.print(TempoStart);
           Serial.print(" conta=");
           Serial.print(conta);
           Serial.print(" Semionde=");
           Serial.println(tante);
              
           if (somma >110 )    //se il valore supera la soglia, qui impostata a 110, allora segnala il troppo assorbimento di corrente.
              { 
              Serial.print("***************");  
              Serial.print("Troppa corrente");
              Serial.print("***************");  
              }
 
         Quantacorrenteoldtempo=millis();  
         }
     }	//end QuantaCorrente

 
