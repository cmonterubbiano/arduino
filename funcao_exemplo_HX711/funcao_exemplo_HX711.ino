/*
      Teste de função em C para funcionamento do CI HX711
      
      Conversor AD para célula de carga


      Eng. Wagner Rambo
      Outubro de 2016
*/

// --- Mapeamento de Hardware ---
#define  ADDO  15   //Data Out
#define  ADSK  14    //SCK


// --- Protótipo das Funções Auxiliares ---
unsigned long ReadCount(); //conversão AD do HX711


// --- Variáveis Globais ---
unsigned long convert;


// --- Configurações Iniciais ---
void setup()
{
   pinMode(ADDO, INPUT_PULLUP);   //entrada para receber os dados
   pinMode(ADSK, OUTPUT);         //saída para SCK
   
   Serial.begin(9600);
  Serial.println(" - inizio -> ");
} //end setup


// --- Loop Infinito ---
void loop()
{
  float conv =39462.6667;
  float tara =8849313;
  Serial.println(" - le ttura -> ");
  convert = ReadCount();
  
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









