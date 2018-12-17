/*
Author: AnalysIR 2004 - http://www.AnalysIR.com/
This sketch is Free to use - without restriction
*/

#include <IRremote.h>

IRsend irsend;

int khz = 38; //NB Change this default value as neccessary to the correct modulation frequency


unsigned int powerOff[] = {3496,1636,484,1228,484,1232,456,396,492,368,460,408,480,1232,460,404,488,372,484,1228,488,1228,484,372,460,1252,460,400,456,408,480,1232,488,1228,456,404,456,1260,460,1256,456,412,484,376,484,1232,484,372,456,404,456,1256,460,400,460,396,488,376,456,408,456,400,488,368,460,404,516,356,460,404,480,380,460,400,484,376,480,384,480,376,488,376,488,372,484,376,484,372,488,376,480,380,456,1256,488,372,484,384,460,396,508,360,508,360,480,1244,484,376,456,404,484,384,456,404,460,1256,484,1232,488,1228,484,368,488,372,456,408,484,380,484,372,492,372,488,372,484,376,484,372,488,1224,484,1228,484,384,456,404,484,384,456,404,484,372,456,408,480,380,460,404,484,1232,484,376,460,1252,488,1228,456,400,484,1232,484,1228,484,376,460,1260,484,372,488,372,480,384,456,408,460,404,456,400,484,376,456,408,484,376,484,372,484,380,480,384,484,376,480,388,480,380,488,372,456,404,460,404,480,376,456,404,488,380,480,380,484,380,484,372,488,376,484,380,456,404,460,400,484,372,488,376,484,376,456,408,484,372,488,372,464,396,484,376,480,380,484,376,484,380,480,384,484,380,456,400,456,404,484,380,480,384,480,384,456,400,484,372,484,380,484,1224,488,1224,484,1232,456,1256,452,408,488,372,456,412,456,404,484};



void setup()
{
  Serial.begin(9600);
  irsend.enableIROut(khz);
  delay(1000);
  Serial.println(F("AnalysIR"));
}

void loop()
{
  irsend.sendRaw(powerOff, sizeof(powerOff) / sizeof(int), khz);
  delay(16); //<<<<<<<<<<<<<<<uncomment here if it doesnt work
  irsend.sendRaw(powerOff, sizeof(powerOff) / sizeof(int), khz);  //<<<<<<<<<<<<<<<uncomment here if it doesnt work

  delay(8000); //send signal every 10 seconds for testing
}
