


byte simbolo_USB[8]={
  B00000,
  B01010,
  B01010,
  B11111,
  B11111,
  B01110,
  B00100,
  B00100
};
byte batteria_8[8] ={
  B01110,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

byte batteria_7[8] ={
  B00000,
  B01110,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

byte batteria_6[8] ={
  B00000,
  B00000,
  B01110,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

byte batteria_5[8] ={
  B00000,
  B00000,
  B00000,
  B01110,
  B11111,
  B11111,
  B11111,
  B11111
};


byte batteria_4[8] ={
  B00000,
  B00000,
  B00000,
  B00000,
  B01110,
  B11111,
  B11111,
  B11111
};

byte batteria_3[8] ={
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B01110,
  B11111,
  B11111
};

byte batteria_2[8] ={
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B01110,
  B11111
};

byte batteria_1[8] ={
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B01110
};



void livello_batteria(void)//livello batteria sta in tastiera così compare sempre il simbolino della batteria in qualsiasi caso
{
  stato_batteria = analogRead(A3);

  stato_batteria = stato_batteria * 1.26 ;// moltiplico per 1,26 perchè collegando una batteria da 9V avevo una lettura analogica di 763 e per avere 960 moltiplico il numero  per 1,26  
  batteria = map(stato_batteria, 0, 1023, 0, 8);
  
  if ( (stato_batteria >= 530 )&& (stato_batteria <= 541 ))
  {
    lcd.setCursor(15, 0);
    lcd.write((byte)0);
    lcd.createChar(0,simbolo_USB);
  }

  else{
    switch(batteria)
    {
    case 0:
    case 1:
      lcd.setCursor(15, 0);
      lcd.write((byte)0);
      lcd.createChar(0,batteria_1);  
      break;
    case 2:
      lcd.setCursor(15, 0);
      lcd.write((byte)0);
      lcd.createChar(0,batteria_2);  
      break;
    case 3:
      lcd.setCursor(15, 0);
      lcd.write((byte)0);
      lcd.createChar(0,batteria_3);  
      break;
    case 4:
      lcd.setCursor(15, 0);
      lcd.write((byte)0);
      lcd.createChar(0,batteria_4);  
      break;
    case 5:
      lcd.setCursor(15, 0);
      lcd.write((byte)0);
      lcd.createChar(0,batteria_5);  
      break;
    case 6:
      lcd.setCursor(15, 0);
      lcd.write((byte)0);
      lcd.createChar(0,batteria_6);  
      break;
    case 7:
      lcd.setCursor(15, 0);
      lcd.write((byte)0);
      lcd.createChar(0,batteria_7);  
      break;
    case 8:
      lcd.setCursor(15, 0);
      lcd.write((byte)0);
      lcd.createChar(0,batteria_8);  
      break;
    }
  }
}


