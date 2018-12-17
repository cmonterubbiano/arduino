// Raw (68): -5462 4500 -4350 650 -1550 600 -1550 650 -1550 600 -550 600 -500 600 -500 550 -500 650 -500 600 -1600 600 -1550 600 -1550 650 -550 600 -500 550 -500 600 -500 600 -550 600 -1550 650 -1550 600 -1550 600 -1650 600 -1600 550 -500 650 -450 600 -550 600 -500 600 -450 650 -450 600 -550 650 -450 600 -1600 550 -1600 600 -1600 600 
// Raw (68): -11490 4500 -4350 600 -1600 600 -1550 600 -1600 550 -600 550 -550 550 -550 550 -500 550 -600 600 -1550 600 -1600 600 -1550 600 -600 550 -550 550 -500 550 -550 550 -600 550 -500 600 -500 600 -1600 550 -600 550 -550 550 -500 600 -500 550 -600 600 -1550 600 -1600 600 -500 550 -1700 550 -1600 550 -1650 550 -1600 550 -1600 600 

// Attenzione l'IR funziona solo se coperto

#include "IRremot4.h"

IRsend irsend;

void setup()

{
  Serial.begin(9600);
}
// codifiche tasti info 1 tasti rosso verde giallo blu
int khz=38; //NB Change this default value as neccessary 
//to the correct modulation frequency

unsigned info[] ={
  4500, 4350, 650, 1550, 600,1550, 650, 1550, 600, 550, 600, 500, 600, 500, 550, 500, 650, 500, 600, 1600, 600, 1550, 600, 1550, 650, 550, 600, 500, 550, 500, 600, 500, 600, 550, 600, 1550, 650, 1550, 600, 1550, 600, 1650, 600, 1600, 550, 500, 650, 450, 600, 550, 600, 500, 600, 450, 650, 450, 600, 550, 650, 450, 600, 1600, 550, 1600, 600, 1600, 600};
unsigned uno[] = {
  4500,4350,600,1600,600,1550,600,1600,550,600,550,550,550,550,550,500,550,600,600,
  1550,600,1600,600,1550,600,600,550,550,550,500,550,550,550,600,550,500,600,500,600,
  1600,550,600,550,550,550,500,600,500,550,600,600,1550,600,1600,600,500,550,1700,550,1600,550,1650,550,1600,550,1600,600};
unsigned tasto_r[] ={
  4550,4300,600,1600,600,1550,600,1600,550,600,600,500,550,500,600,500,600, 
  550,600,1550,650,1550,550,1600,650,550,550,500,550,550,600,500,550,600,600,450,600,
  500,600,1600,550,1650,600,500,600,1600,550,1600,600,550,600,1550,650,1550,600,500,550, 
  600,600,1550,600,500,550,550,600,1550,600};
unsigned tasto_v[] ={
  4500,4350,650,1550,600,1550,650,1550,600,550,550,550,600,500,550,500,600, 
  550,600,1600,600,1550,600,1600,600,550,550,550,550,500,600,500,600,550,600,500,600,
  450,600,1600,600,550,550,1650,550,500,550,550,550,600,600,1550,600,1600,550,500,600,
  1650,600,500,600,1600,550,1600,600,1600,550};
unsigned tasto_g[]  ={
  4500,4350,600,1600,600,1550,600,1600,600,550,600,500,550,500,600,500,550, 
  600,600,1550,600,1600,600,1550,600,600,550,500,550,550,600,500,550,600,550,1600,600,
  500,600,1550,600,550,600,1600,550,550,550,500,600,550,600,500,600,1550,650,450,600,
  1650,550,550,550,1600,600,1600,550,1600,600};
unsigned tasto_b[]  ={
  4500,4350,600,1600,550,1600,600,1600,600,550,550,550,550,500,600,500,600, 
  550,600,1550,650,1550,600,1550,650,550,550,500,600,500,550,550,550,600,600,450,650,
  1550,600,1550,600,600,550,1600,600,500,550,550,550,550,650,1550,600,450,650,450,600,
  1650,600,500,600,1550,600,1600,600,1550,600}; 

void loop()
{
  Serial.println("invio info");
  irsend.sendRap(info, sizeof(info)/sizeof(int), khz, 4);
  delay(5000);
  Serial.println("invio uno");
  irsend.sendRap(uno, sizeof(uno)/sizeof(int),khz);
  delay(5000);
  Serial.println("invio R");
  irsend.sendRap(tasto_r, sizeof(tasto_r)/sizeof(int),khz);
  delay(5000);
  Serial.println("invio V");
  irsend.sendRap(tasto_v, sizeof(tasto_v)/sizeof(int),khz);
  delay(5000);
  Serial.println("invio G");
  irsend.sendRap(tasto_g, sizeof(tasto_g)/sizeof(int),khz);
  delay(5000);
  Serial.println("invio B");
  irsend.sendRap(tasto_b, sizeof(tasto_b)/sizeof(int),khz);
  delay(5000);
}
