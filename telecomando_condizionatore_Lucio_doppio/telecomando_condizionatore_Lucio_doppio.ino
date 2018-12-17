// Raw (68): -5462 4500 -4350 650 -1550 600 -1550 650 -1550 600 -550 600 -500 600 -500 550 -500 650 -500 600 -1600 600 -1550 600 -1550 650 -550 600 -500 550 -500 600 -500 600 -550 600 -1550 650 -1550 600 -1550 600 -1650 600 -1600 550 -500 650 -450 600 -550 600 -500 600 -450 650 -450 600 -550 650 -450 600 -1600 550 -1600 600 -1600 600 
// Raw (68): -11490 4500 -4350 600 -1600 600 -1550 600 -1600 550 -600 550 -550 550 -550 550 -500 550 -600 600 -1550 600 -1600 600 -1550 600 -600 550 -550 550 -500 550 -550 550 -600 550 -500 600 -500 600 -1600 550 -600 550 -550 550 -500 600 -500 550 -600 600 -1550 600 -1600 600 -500 550 -1700 550 -1600 550 -1650 550 -1600 550 -1600 600 

// Attenzione l'IR funziona solo se coperto

#include "IRremote.h"

IRsend irsend;

void setup()

{
  Serial.begin(9600);
}
// codifiche tasti info 1 tasti rosso verde giallo blu
int khz=38; //NB Change this default value as neccessary 
            //to the correct modulation frequency
            
unsigned acceso [] ={3350,1600,450,1200,450,1200,450,400,450,350,450,400,450,1200,
                    450,350,450,400,450,1200,450,1200,450,350,500,1150,500,350,450,
                    350,500,1200,450,1200,450,350,450,1200,450,1200,450,400,450,350,
                    450,1200,500,350,450,350,500,1150,500,350,450,350,500,350,450,350,
                    500,350,450,350,500,350,450,350,500,350,450,400,450,350,450,400,
                    450,350,450,400,450,350,450,400,450,350,450,400,450,350,450,400,
                    450,1200,450,350,500,350,450};
                    
unsigned con_on [] ={3350,1600,450,1200,450,1200,450,400,450,350,450,400,450,1200,
                    450,350,500,350,450,1200,450,1200,450,400,450,1200,450,350,450,
                    400,450,1200,450,1200,450,350,450,1200,450,1200,450,400,450,350,
                    500,1150,500,350,450,350,500,1200,450,350,450,400,450,350,450,
                    400,450,350,450,400,450,350,450,400,450,350,450,400,450,350,450,
                    400,450,350,450,400,450,350,500,350,450,350,500,350,450,350,500,
                    350,450,1200,450,400,450,350,450};

unsigned spento [] ={3350,1600,500,1150,500,1150,500,350,450,400,450,350,450,1200,
                    450,400,450,350,450,1200,450,1200,450,400,450,1200,450,350,500,
                    350,450,1200,450,1200,450,350,500,1200,450,1200,450,350,450,400,
                    450,1200,450,350,450,400,450,1200,450,350,450,400,450,350,500,
                    350,450,350,500,350,450,350,500,350,450,350,500,350,450,350,500,
                    350,450,400,450,350,450,400,450,350,450,400,450,350,450,400,450,
                    350,450,400,450,350,450,400,450};
                    
unsigned con_of [] ={3350,1600,500,1150,500,1200,450,350,450,400,450,350,450,1200,
                    450,400,450,350,450,1200,450,1200,450,400,450,1200,450,350,450,
                    400,450,1200,450,1200,450,400,450,1150,500,1200,450,350,450,400,
                    450,1200,450,350,450,400,450,1200,450,350,450,400,450,350,450,
                    400,450,350,450,400,450,350,500,350,450,350,500,350,450,350,500,
                    350,450,400,450,350,450,400,450,350,450,400,450,350,450,400,450,
                    350,450,400,450,350,450,400,450};


void loop()
{
  Serial.println("Accendo .....");
  irsend.sendRaw(acceso, sizeof(acceso)/sizeof(int), khz);
  delay(10);
  irsend.sendRaw(con_on, sizeof(con_on)/sizeof(int), khz);
  delay(7000);
  Serial.println("spengo");
  irsend.sendRaw(spento, sizeof(spento)/sizeof(int),khz);
  delay(10);
  irsend.sendRaw(con_of, sizeof(con_of)/sizeof(int), khz);
  delay(7000);
}