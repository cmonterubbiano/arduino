#define  NA  0

// include the library code:
#include <pulsanti.h>

void setup()
{
  int  iy;
  int  iz;
  
  for (iy =0; iy <NARD; iy++)
    *trasmi[iy] ='\0';
    
  Serial.begin(9600);
  if (!NA)
  {
    Serial1.begin(9600);
    Serial2.begin(9600);
  }
  else  if (NA ==1)
    Serial1.begin(9600);
  else  if (NA ==2)
    Serial2.begin(9600);
  delay(20);
  for (pulini =iy =0; iy <NA; pulini +=npul[iy], iy++);
  Serial.print("IX ");
  Serial.print(npul[NA]);
  Serial.print(" IZ ");
  Serial.println(pulini);
  iz =pulini;

  for (iy =iz +npul[NA]; iz <iy; iz++)
  {
    if (!ppin[iz])
      continue;
    pinMode(ppin[iz], INPUT);
    Serial.print("INPUT ");
    Serial.println(ppin[iz]);
    digitalWrite(ppin[iz], HIGH);
    delay(20);
  }

  for (iy =iz =0; iy <NA; iz +=rele[iy], iy++);
  relini =iz;
  Serial.begin(9600);
  for (iy =iz +rele[NA]; iz <iy; iz++)
  {
    if (!rpin[iz])
      continue;
      
    pinMode(rpin[iz], OUTPUT);
    Serial.print("OUTPUT ");
    Serial.print(rpin[iz]);
    delay(20);
    digitalWrite(rpin[iz], (!roff[iz] ? LOW : HIGH));
    Serial.print(" OFF ");
    Serial.println((!roff[iz] ? "LOW" : "HIGH"));
    delay(20);
  }
  Serial.println("FINE");
}

void loop()
{
  int  pulsa;
  int  ix;
  
  for(pulsa =0; pulsa <npul[NA]; pulsa++)
  {
    int  pulsante =pulini +pulsa;
    
    if (!ppin[pulsante])
      continue;
    
    int  pulstat =digitalRead(ppin[pulsante]);
    
    if (!pulstat)                                // Pulsante premuto
    {
      if (!spul[pulsante])                       // Se il pulsante non era gia' premuto
      {
        spul[pulsante] =millis();                // Memorizzo l'istante in cui e' stao premuto
        Serial.print("ON Pulsante ");
        Serial.println(ppin[pulsante]);
      }
    }
    else  if (spul[pulsante])                    // Se il pulsante non e' premuto ma era premuto (rilasciato)
    {
      unsigned  long  diff =(millis() -spul[pulsante]);
      
      spul[pulsante] =0;
      
      if (diff >TMIN  && diff <TREP)            // Se il tempo di pressione e' normale            
      {
        int  esegui =1;
        
        Serial.print("Tempo normale ");
        Serial.print("Pulsante ");
        Serial.print(pulsante);
        Serial.print(" papu ");
        Serial.println(papu[pulsante]);
        if (papu[pulsante])                              // Se il pulsante e' abbinato ad altro pulsante
        {
          if (appu[pulsante] ==NA)                       // Se il pulsante e' abbinato sulla stessa scheda               
          {
            Serial.print("Pulsante prima ");
            Serial.print(ppin[pulsante]);
            psta[pulsante] =0;
            pulsante =(pulini +papu[pulsante] -1);
            Serial.print(" Pulsante dopo ");
            Serial.println(ppin[pulsante]);
          }
          else                                          // Se il pulsante e' abbinato ad una scheda diversa
          {
            char  mess[10];
             
            esegui =0;
            sprintf(mess, "p,%02d", papu[pulsante]);
            strcpy(&trasmi[(appu[pulsante])][(strlen(trasmi[(appu[pulsante])]))], mess);
            Serial.print("Stack da trasferire ");
            Serial.println(mess);
          }
        } 
        if (esegui)                                       // se il pulsante deve essere preso in esame
        {
          int  posrel =((pulsante -pulini) +(NA *NREL));  // posizione del pulsante relativa.
          int  sequenza;
          int  iy;
          
          if (!psta[pulsante])                            // Se lo stato attuale del pulsante e' zero
            psta[pulsante] =1;
          else
          {
            psta[pulsante] =(psta[pulsante] <<1);         // Incremento stato pulsante (1-2-4-8-16 etc.)
            Serial.print(" Dopo shift ");
            Serial.println(psta[pulsante]);
            if (psta[pulsante] >sepu[pulsante])           // Se il nuovo stato non e' previsto viente riportato all'iniziale
              psta[pulsante] =0;
            Serial.print(" Dopo sequ ");
            Serial.println(psta[pulsante]);
          }
          sequenza =psta[pulsante];                       // Situazione da ottenere

          for (iy =0; iy <(NARD *NREL); iy++)              // Per tutti i rele di tutte le schede
          {
            if (!rabb[iy][posrel])                         // Se il pulsante non ha nessun effetto =0
              continue;
              
            Serial.print("iy ");
            Serial.print(iy);
            Serial.print(" posrel ");
            Serial.print(" rabb ");
            Serial.print(rabb[iy][posrel]);
            Serial.print(" sequenza ");
            Serial.print(sequenza);

            if (rabb[iy][posrel] & sequenza)              // Se il numero e' in and con la sequenza Es 3 e' in and sia con 1 che 2
            {
              if (iy >=(NA *NREL) && iy <(NA * NREL +NREL))  // Se il rele fa parte della scheda attuale
              {
                int  nrel =rpin[(iy -(NA *NREL) +relini)];
                digitalWrite(nrel, (roff[(iy -(NA *NREL) +relini)] ? LOW : HIGH));
                rsta[(iy -(NA *NREL) +relini)] =1;
                Serial.print(" Rele HIGH ");
                Serial.println(nrel);
                cambiostato++;
              }
              else                                            // Se il rele fa parte du una scheda diversa
              {
                char  mess[10];
             
                sprintf(mess, "r,%02d,1", (iy % NREL));
                strcpy(&trasmi[((iy /NREL))][(strlen(trasmi[((iy /NREL))]))], mess);
                Serial.print("Stack da trasferire ");
                Serial.println(mess);
              }
            }
            else                                             // Se il numero non e' in and con la sequenza
            {
              if (iy >=(NA *NREL) && iy <(NA * NREL +NREL))  // Se il rele fa parte della scheda attuale
              {
                int  nrel =rpin[(iy -(NA *NREL) +relini)];
                digitalWrite(nrel, (!roff[(iy -(NA *NREL) +relini)] ? LOW : HIGH));
                rsta[(iy -(NA *NREL) +relini)] =0;
                Serial.print(" Rele LOW ");
                Serial.println(nrel);
                cambiostato++;
              }
              else                                            // Se il rele fa parte du una scheda diversa
              {
                char  mess[10];
             
                sprintf(mess, "r,%02d,0", (iy % NREL));
                strcpy(&trasmi[((iy /NREL))][(strlen(trasmi[((iy /NREL))]))], mess);
                Serial.print("Stack da trasferire ");
                Serial.println(mess);
              }
            }
          }
        }
      }
      else  if (diff >=TREP && diff <TREG)        // Se il tempo supera i "3" secondi
      {
        Serial.print("Reset parziale ");
      }
      else  if (diff >=TREG)                      // Se il tempo supera i "10" secondi
      {
        Serial.print("Reset generale ");
      }
      Serial.print("Off Pulsante ");
      Serial.print(ppin[pulsante]);
      Serial.print(" tempo ");
      Serial.println(diff);
    }
    delay(1);
  }
  for (ix =0; ix <NARD; ix++)
  {
    if (*trasmi[ix])
	esegui_trasf(ix);
  }
  if (cambiostato && NA != MAST)
  {
    int  iy;
    
    Serial.print("Stato rele' -> ");
    for (iy =0,ix =relini; iy <rele[NA]; iy++)
    {
      Serial.print(rsta[(relini+iy)]);
      if (NA ==1)
      {
        if (!iy)
          Serial1.print("X-");
        Serial1.print(rsta[(relini+iy)]);
      }
      if (NA ==2)
      {
        if (!iy)
          Serial2.print("X-");
        Serial2.print(rsta[(relini+iy)]);
      }
    }
    if (NA ==1)
        Serial1.println("");
    if (NA ==2)
        Serial2.println("");
    Serial.println("");
    cambiostato =0;
  }
}

void esegui_trasf(int  ix)
{
  if (ix !=NA)
  {
    int  iy;
    
    Serial.print("Buffer da trasferire ");
    Serial.print(ix);
    Serial.print(" - ");
    Serial.println(trasmi[ix]);
    if (NA ==MAST)
      iy =ix;
    else  iy =NA;
    if (iy ==1)
      Serial1.println(trasmi[ix]);
    if (iy ==2)
      Serial2.println(trasmi[ix]);
  }
  else
  {
  }
  *trasmi[ix] ='\0';
}

char  appo[256];

void serialEvent1()
{
  *appo ='\0';
  while (Serial1.available())
  {
    char inChar = (char)Serial1.read();
    
    if (inChar == '\n')
      break;
    else  if (strlen(appo)<256)
    {
      sprintf(&appo[(strlen(appo))], "%c", inChar);
    }
    delay(10);
  }
  Serial.print("ARRIVATO 1-> ");
  Serial.println(appo);
  if (toupper(*appo) =='X')
  {
    int  ix;
    int  iy;
    
    for (ix =rele[0] ; ix <(rele[0] +rele[1]); ix++)
    {
      iy =ix -rele[0] +2;

      rsta[ix] =(appo[iy] -'0');
    }
    for (ix =iy =0; ix <NARD; iy +=rele[ix++]);
    Serial.print("Stato rele' generale -> ");
    for (ix =0; ix <iy; ix++)
      Serial.print(rsta[ix]);

    Serial.println("");
  }
  else
  {
    int  ix =(*appo - '0');
    
    if (ix >=0 && ix <NARD)
    {
      strcpy(&trasmi[ix][(strlen(trasmi[ix]))], &appo[2]);
      Serial.print("Stato buffer ");
      Serial.print(ix);
      Serial.print(" - ");
      Serial.println(trasmi[ix]);
    }
  }
}

void serialEvent2()
{
  while (Serial2.available())
  {
    char inChar = (char)Serial2.read();
    
    if (inChar == '\n')
      break;
    else  if (strlen(trasmi[NA])<256)
    {
      sprintf(&trasmi[NA][(strlen(trasmi[NA]))], "%c", inChar);
    }
    delay(10);
  }
  Serial.print("ARRIVATO 2-> ");
  Serial.println(trasmi[NA]); 
}
