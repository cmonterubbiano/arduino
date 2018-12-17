//#include <IRLib.h>
#include <IRremote.h>

IRsend My_Sender;
    
  //mitsubishi 23 gradi ventola 2 solo riscladamento  
  unsigned long signal_1[] = {0xc4d36480, 0x410e0, 0xc425400, 0x0, 0x70000}; 

  //mitsubishi 23 gradi ventola 4 solo riscladamento
  unsigned long signal_2[] = {0xc4d36480, 0x410e0, 0xc225400, 0x0, 0x470000}; 

  unsigned long signal_3[] = {0xc4d36480, 0x410e0, 0xc425400, 0x0, 0x70000}; 

  unsigned long signal_4[] = {0xc4d36480, 0x410e0, 0xc425400, 0x0, 0x70000}; 

  
  int khz=38; //NB Change this default value as neccessary to  38, 33, 36, 40, 56
  int led = 13;
  int ix;

void setup()
{
  


  int ix;
  Serial.begin(9600);
  delay(5000);
  //Serial.println(sizeof(Accensione_0_0) / sizeof(int));
  pinMode(led, OUTPUT);
  
  

  condMitsubishiComando_2();

}

void loop() {
  
}

void condMitsubishiComando_1() {

  for(ix=0;ix<3;ix++){
	  //usually in Loop
	  digitalWrite(led, HIGH);
          My_Sender.sendMitsubishi(signal_1[0],signal_1[1],signal_1[2],signal_1[3],signal_1[4]);
	  delay(1000);
  	  digitalWrite(led, LOW);
	  delay(10000);   
  }  
  
}


void condMitsubishiComando_2() {

  for(ix=0;ix<3;ix++){
	  //usually in Loop
	  digitalWrite(led, HIGH);
          My_Sender.sendMitsubishi(signal_2[0],signal_2[1],signal_2[2],signal_2[3],signal_2[4]);
	  delay(1000);
  	  digitalWrite(led, LOW);
	  delay(10000);   
  }  
  
}

