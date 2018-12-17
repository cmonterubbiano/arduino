/*
      Teste de função em C para funcionamento do CI HX711
      
      Conversor AD para célula de carga


      Eng. Wagner Rambo
      Outubro de 2016
	  
	  resistenze da 1Kh tra E+ ed A+ e tra E- ed A+
	  la bilacia va collegata 	Bianco	su E+
								Nero 	su E-
								Rosso	su A-
	  HX711 a parte massa e +5  va collegata in qualsiasi pin digitale
*/

// --- Mapeamento de Hardware ---
#define  ADDO  15   //Data Out
#define  ADSK  14    //SCK
#define  ADDO1  17   //Data Out
#define  ADSK1  16    //SCK
#define  ADDO2  19   //Data Out
#define  ADSK2  18    //SCK
#define  ADDO3  3   //Data Out
#define  ADSK3  2    //SCK

#define  SEN1  7   //Data Out


// --- Protótipo das Funções Auxiliares ---
unsigned long ReadCount(); //conversão AD do HX711


// --- Variáveis Globais ---
unsigned long convert;


// --- Configurações Iniciais ---
void setup()
{
   pinMode(ADDO, INPUT_PULLUP);   //entrada para receber DT
   pinMode(ADSK, OUTPUT);         //saída para SCK
   pinMode(ADDO1, INPUT_PULLUP);   //entrada para receber os dados
   pinMode(ADSK1, OUTPUT);         //saída para SCK
   pinMode(ADDO2, INPUT_PULLUP);   //entrada para receber os dados
   pinMode(ADSK2, OUTPUT);         //saída para SCK
   pinMode(ADDO3, INPUT_PULLUP);   //entrada para receber os dados
   pinMode(ADSK3, OUTPUT);         //saída para SCK
   
   pinMode(SEN1, INPUT_PULLUP);   //1' sensore
   
   Serial.begin(9600);
} //end setup


// --- Loop Infinito ---
void loop()
{
	if (!digitalRead(SEN1))
		Serial.println("Tasto premuto");
		
  float conv =39462.6667;     // Valore dichiarato diviso peso conosciuto
  float tara =8849313;        // valore dichiarato senza nessun peso diciamo in condizioni di riposo
  
  convert = ReadCount();
  
  Serial.print(convert);
  Serial.print(" - convertito con fattore -> ");
  Serial.print((convert -tara) /conv);
  
  Serial.print(" <- seconda bilancia -> ");
  
  conv = -44673.33;
  tara =6804645;
  
  convert = ReadCount1();
	
  Serial.print(convert);
  Serial.print(" - convertito con fattore -> ");
  Serial.println((convert -tara) /conv);
 
   delay(1000);
  conv = 1;
  tara =10536902;
  
  convert = ReadCount2();
	
  Serial.print(convert);
  Serial.print(" - convertito con fattore -> ");
  Serial.print((convert -tara) /conv);
  Serial.print(" <- terza bil- quarta-> ");
  
  conv = -020667;
  tara =6814522;
  
  convert = ReadCount3();
	
  Serial.print(convert);
  Serial.print(" - convertito con fattore -> ");
  Serial.println((convert -tara) /conv);
  
  delay(2000);
  


} //end loop


// --- Funções ---
unsigned long ReadCount()
{
  unsigned long Count = 0;
  unsigned char i;
  
  digitalWrite(ADSK, LOW);

  while(digitalRead(ADDO));

  for(i=0;i<24;i++)
  {
     digitalWrite(ADSK, HIGH);
     Count = Count << 1;
     digitalWrite(ADSK, LOW);
     if(digitalRead(ADDO)) Count++;
  
  } //end for
  
  digitalWrite(ADSK, HIGH);
  Count = Count^0x800000;
  digitalWrite(ADSK, LOW);
  
  return(Count);


} //end ReadCount

// --- Funções ---
unsigned long ReadCount1()
{
  unsigned long Count = 0;
  unsigned char i;
  
  digitalWrite(ADSK1, LOW);

  while(digitalRead(ADDO1));

  for(i=0;i<24;i++)
  {
     digitalWrite(ADSK1, HIGH);
     Count = Count << 1;
     digitalWrite(ADSK1, LOW);
     if(digitalRead(ADDO1)) Count++;
  
  } //end for
  
  digitalWrite(ADSK1, HIGH);
  Count = Count^0x800000;
  digitalWrite(ADSK1, LOW);
  
  return(Count);


} //end ReadCount
// --- Funções ---
unsigned long ReadCount2()
{
  unsigned long Count = 0;
  unsigned char i;
  
  digitalWrite(ADSK2, LOW);

  while(digitalRead(ADDO2));

  for(i=0;i<24;i++)
  {
     digitalWrite(ADSK2, HIGH);
     Count = Count << 1;
     digitalWrite(ADSK2, LOW);
     if(digitalRead(ADDO2)) Count++;
  
  } //end for
  
  digitalWrite(ADSK2, HIGH);
  Count = Count^0x800000;
  digitalWrite(ADSK2, LOW);
  
  return(Count);


} //end ReadCount
// --- Funções ---
unsigned long ReadCount3()
{
  unsigned long Count = 0;
  unsigned char i;
  
  digitalWrite(ADSK3, LOW);

  while(digitalRead(ADDO3));

  for(i=0;i<24;i++)
  {
     digitalWrite(ADSK3, HIGH);
     Count = Count << 1;
     digitalWrite(ADSK3, LOW);
     if(digitalRead(ADDO3)) Count++;
  
  } //end for
  
  digitalWrite(ADSK3, HIGH);
  Count = Count^0x800000;
  digitalWrite(ADSK3, LOW);
  
  return(Count);


} //end ReadCount







