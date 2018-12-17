
/*
Author: AnalysIR 2004 - http://www.AnalysIR.com/
This sketch is Free to use - without restriction
*/

#include <IRremote.h>

IRsend irsend;

//You could also try 36,40 & 56 frequencies as a test NORMAL = 38
int khz = 38; //NB Change this default value as neccessary to the correct modulation frequency

//mio spegnimento dimezzato:
//unsigned int Signal_0_0[] = {3472, 1660, 480, 1232, 468, 1240, 468, 396, 464, 396, 484, 384, 464, 1244, 456, 408, 464, 396, 464, 1244, 488, 1228, 464, 396, 456, 1252, 472, 388, 456, 408, 488, 1232, 452, 1260, 464, 400, 452, 1260, 488, 1228, 456, 408, 488, 376, 464, 1248, 468, 396, 448, 408, 468, 1244, 464, 396, 484, 380, 456, 408, 464, 392, 464, 392, 456, 408, 480, 380, 456, 408, 452, 412, 452, 408, 468, 388, 484, 380, 464, 392, 468, 396, 480, 380, 464, 396, 464, 392, 452, 408, 464, 396, 488, 376, 464, 396, 456, 408, 480, 384, 464, 392, 464, 396, 456, 404, 464, 1252, 480, 380, 456, 404, 464, 396, 464, 400, 464, 1248, 468, 1240, 468, 1248, 464, 396, 464, 392, 412, 452, 408, 452, 480, 384, 464, 396, 464, 392, 464, 396, 456, 408, 456, 1260, 464, 1244, 456, 408, 468, 392, 464, 396, 484, 380, 456, 408, 464, 396, 464, 400, 452, 412, 464, 1248, 464, 400, 480, 1236, 464, 1252, 404, 456, 456, 404, 464, 1244, 496, 1232, 408, 456, 464, 400, 408, 452, 464, 396, 460, 400, 408, 456, 464, 396, 464, 396, 464, 404, 408, 456, 460, 404, 404, 456, 456, 400, 404, 460, 456, 404, 408, 456, 484, 384, 460, 404, 404, 460, 452, 408, 464, 396, 456, 404, 464, 400, 460, 400, 404, 456, 460, 396, 468, 392, 464, 396, 408, 456, 460, 400, 460, 404, 456, 412, 460, 400, 404, 452, 460, 400, 496, 380, 488, 376, 408, 456, 460, 400, 408, 456, 436, 428, 460, 400, 464, 396, 464, 396, 460, 396, 464, 400, 408, 452, 408, 456, 408, 452, 428, 432, 460, 1252, 484, 1240, 460, 1244, 464, 396, 432, 432, 404, 456, 408, 1304, 460, 1252, 408}; //AnalysIR Batch Export - RAW   
//televisione volume - funziona:
//unsigned int Signal_0_0[] = {4528, 4420, 596, 1628, 596, 1628, 596, 1628, 592, 520, 596, 516, 596, 516, 596, 516, 592, 520, 592, 1632, 596, 1624, 596, 1628, 596, 516, 596, 520, 592, 516, 600, 512, 596, 516, 596, 1628, 596, 1624, 596, 520, 596, 1628, 596, 512, 600, 512, 596, 520, 592, 520, 596, 516, 596, 512, 600, 1624, 596, 520, 592, 1632, 596, 1624, 596, 1628, 596, 1624, 596, 46392, 4524, 4424, 596, 1628, 596, 1628, 596, 1624, 596, 520, 592, 520, 596, 512, 600, 512, 596, 520, 592, 1628, 596, 1628, 592, 1628, 596, 520, 596, 516, 596, 512, 600, 512, 596, 520, 592, 1632, 596, 1624, 596, 516, 596, 1628, 596, 516, 596, 516, 592, 520, 592, 520, 596, 516, 596, 516, 596, 1628, 592, 520, 596, 1628, 596, 1624, 596, 1628, 596, 1628, 596}; //AnalysIR Batch Export  RAW   
//mio spegnimento completo:
unsigned int Signal_0_1[] = {3472, -1660, 480, -1232, 468, -1240, 468, -396, 464, -396, 484, -384, 464, -1244, 456, -408, 464, -396, 464, -1244, 488, -1228, 464, -396, 456, -1252, 472, -388, 456, -408, 488, -1232, 452, -1260, 464, -400, 452, -1260, 488, -1228, 456, -408, 488, -376, 464, -1248, 468, -396, 448, -408, 468, -1244, 464, -396, 484, -380, 456, -408, 464, -392, 464, -392, 456, -408, 480, -380, 456, -408, 452, -412, 452, -408, 468, -388, 484, -380, 464, -392, 468, -396, 480, -380, 464, -396, 464, -392, 452, -408, 464, -396, 488, -376, 464, -396, 456, -408, 480, -384, 464, -392, 464, -396, 456, -404, 464, -1252, 480, -380, 456, -404, 464, -396, 464, -400, 464, -1248, 468, -1240, 468, -1248, 464, -396, 464, -392, 412, -452, 408, -452, 480, -384, 464, -396, 464, -392, 464, -396, 456, -408, 456, -1260, 464, -1244, 456, -408, 468, -392, 464, -396, 484, -380, 456, -408, 464, -396, 464, -400, 452, -412, 464, -1248, 464, -400, 480, -1236, 464, -1252, 404, -456, 456, -404, 464, -1244, 496, -1232, 408, -456, 464, -400, 408, -452, 464, -396, 460, -400, 408, -456, 464, -396, 464, -396, 464, -404, 408, -456, 460, -404, 404, -456, 456, -400, 404, -460, 456, -404, 408, -456, 484, -384, 460, -404, 404, -460, 452, -408, 464, -396, 456, -404, 464, -400, 460, -400, 404, -456, 460, -396, 468, -392, 464, -396, 408, -456, 460, -400, 460, -404, 456, -412, 460, -400, 404, -452, 460, -400, 496, -380, 488, -376, 408, -456, 460, -400, 408, -456, 436, -428, 460, -400, 464, -396, 464, -396, 460, -396, 464, -400, 408, -452, 408, -456, 408, -452, 428, -432, 460, -1252, 484, -1240, 460, -1244, 464, -396, 432, -432, 404, -456, 408, -1304, 460, -1252, 408, -16948, 3580, -1692, 488, -1220, 456, -1260, 516, -344, 488, -368, 484, -384, 508, -1204, 412, -452, 496, -364, 512, -1204, 484, -1236, 452, -408, 488, -1220, 492, -372, 456, -404, 492, -1224, 488, -1224, 508, -356, 516, -1200, 480, -1232, 492, -368, 480, -384, 488, -1220, 516, -344, 484, -376, 456, -1260, 512, -344, 492, -368, 488, -372, 456, -408, 516, -344, 488, -364, 492, -372, 520, -352, 488, -376, 512, -352, 492, -368, 488, -372, 480, -380, 488, -372, 488, -372, 492, -372, 484, -376, 484, -372, 488, -372, 484, -376, 456, -408, 408, -452, 460, -404, 488, -372, 516, -340, 492, -372, 484, -1232, 488, -376, 484, -368, 492, -368, 492, -368, 488, -1228, 480, -1232, 456, -1260, 456, -400, 512, -352, 488, -376, 492, -368, 460, -404, 488, -372, 484, -372, 412, -448, 464, -396, 484, -1236, 456, -1256, 492, -368, 488, -376, 488, -376, 480, -388, 480, -384, 456, -412, 488, -376, 492, -368, 488, -1228, 484, -368, 492, -1224, 484, -1232, 468, -392, 488, -380, 492, -1220, 488, -1224, 488, -368, 488, -376, 484, -376, 488, -372, 488, -368, 488, -372, 492, -368, 428, -436, 484, -372, 460, -404, 460, -400, 484, -376, 492, -372, 484, -372, 488, -376, 516, -356, 484, -372, 456, -420, 480, -380, 456, -404, 488, -372, 488, -372, 488, -376, 484, -376, 452, -404, 456, -404, 460, -404, 488, -372, 488, -372, 484, -380, 484, -372, 488, -376, 488, -376, 488, -376, 456, -404, 456, -404, 488, -376, 480, -380, 484, -380, 488, -368, 488, -372, 488, -376, 484, -372, 488, -372, 492, -372, 456, -408, 456, -408, 484, -372, 484, -380, 484, -372, 512, -1208, 480, -1232, 456, -1260, 484, -376, 484, -376, 488, -376, 484, -1224, 460, -1256, 484}; //AnalysIR Batch Export  RAW   
// 24 gradi dimezzato:
unsigned int Signal_0_2[] = {3440, 1688, 456, 1260, 464, 1244, 460, 404, 460, 400, 456, 412, 460, 1248, 408, 456, 460, 400, 460, 1248, 456, 1260, 464, 396, 408, 1304, 460, 396, 408, 460, 404, 1308, 408, 1308, 460, 404, 408, 1304, 456, 1264, 460, 404, 460, 404, 456, 1252, 460, 400, 464, 392, 468, 1248, 440, 420, 456, 408, 408, 452, 460, 400, 460, 404, 464, 396, 460, 408, 460, 400, 464, 392, 464, 396, 460, 400, 464, 400, 404, 460, 460, 392, 408, 456, 456, 404, 456, 408, 460, 400, 464, 392, 460, 404, 492, 1240, 404, 456, 464, 400, 464, 396, 460, 400, 460, 404, 408, 1304, 468, 404, 456, 412, 460, 396, 408, 456, 460, 400, 488, 388, 484, 380, 464, 1248, 460, 404, 460, 400, 428, 436, 456, 400, 412, 452, 456, 408, 408, 456, 456, 408, 456, 1256, 460, 1256, 460, 400, 408, 456, 464, 396, 460, 400, 460, 404, 424, 436, 456, 412, 460, 396, 460, 1252, 440, 420, 460, 1252, 460, 1252, 460, 1252, 460, 400, 404, 1304, 412, 1308, 456, 408, 408, 456, 460, 412, 460, 396, 460, 400, 408, 452, 456, 412, 460, 404, 424, 436, 464, 396, 460, 400, 460, 400, 460, 400, 456, 408, 408, 456, 460, 396, 460, 404, 460, 400, 464, 396, 456, 408, 404, 452, 432, 432, 456, 408, 408, 460, 408, 456, 460, 404, 460, 396, 460, 400, 460, 404, 408, 456, 440, 420, 460, 400, 460, 412, 460, 396, 460, 404, 460, 400, 460, 400, 460, 404, 408, 448, 412, 452, 460, 408, 460, 404, 460, 404, 428, 440, 460, 392, 456, 408, 460, 400, 464, 396, 440, 424, 460, 400, 456, 400, 460, 400, 460, 1252, 508, 1208, 492, 364, 480, 1236, 516, 1204, 480, 1236, 480}; //AnalysIR Batch Export  RAW   
//from AnalysIR forum
unsigned int Signal_0_3[] = {3496,1636,484,1228,484,1232,456,396,492,368,460,408,480,1232,460,404,488,372,484,1228,488,1228,484,372,460,1252,460,400,456,408,480,1232,488,1228,456,404,456,1260,460,1256,456,412,484,376,484,1232,484,372,456,404,456,1256,460,400,460,396,488,376,456,408,456,400,488,368,460,404,516,356,460,404,480,380,460,400,484,376,480,384,480,376,488,376,488,372,484,376,484,372,488,376,480,380,456,1256,488,372,484,384,460,396,508,360,508,360,480,1244,484,376,456,404,484,384,456,404,460,1256,484,1232,488,1228,484,368,488,372,456,408,484,380,484,372,492,372,488,372,484,376,484,372,488,1224,484,1228,484,384,456,404,484,384,456,404,484,372,456,408,480,380,460,404,484,1232,484,376,460,1252,488,1228,456,400,484,1232,484,1228,484,376,460,1260,484,372,488,372,480,384,456,408,460,404,456,400,484,376,456,408,484,376,484,372,484,380,480,384,484,376,480,388,480,380,488,372,456,404,460,404,480,376,456,404,488,380,480,380,484,380,484,372,488,376,484,380,456,404,460,400,484,372,488,376,484,376,456,408,484,372,488,372,464,396,484,376,480,380,484,376,484,380,480,384,484,380,456,400,456,404,484,380,480,384,480,384,456,400,484,372,484,380,484,1224,488,1224,484,1232,456,1256,452,408,488,372,456,412,456,404,484,16383,3604,1656,460,1252,456,1260,460,396,488,380,488,372,480,1236,460,396,484,380,484,1232,484,1228,460,400,456,1256,484,376,484,380,484,1228,480,1232,480,380,488,1232,480,1232,480,388,480,384,484,1224,460,400,484,376,508,1200,460,404,480,380,484,376,484,376,456,408,480,380,460,400,488,376,456,404,484,376,484,372,484,380,488,372,480,384,484,368,488,376,484,376,484,376,456,404,484,376,508,1212,456,400,484,376,488,372,484,376,456,408,480,1228,484,372,484,384,456,404,452,408,456,1256,456,1256,452,1260,480,380,456,408,484,372,460,400,484,376,512,364,480,376,484,384,484,376,480,1228,484,1228,480,384,484,372,456,404,456,404,484,372,460,400,488,376,480,380,456,1252,488,372,484,1228,484,1236,480,376,452,1264,480,1232,484,380,488,1228,456,404,484,376,460,400,456,408,484,372,460,404,484,376,480,376,480,384,452,404,456,408,484,380,480,384,456,416,480,384,484,400,456,376,484,380,512,360,484,384,480,380,484,376,484,380,460,404,480,404,436,404,456,400,484,376,456,404,480,384,448,436,428,408,484,380,480,380,480,400,460,376,456,436,428,408,460,404,456,404,456,400,456,436,432,428,456,388,456,436,456,384,452,404,456,404,456,428,460,1228,456,1256,480,1256,432,1284,456,380,456,404,456,408,456,408,480}; //AnalysIR Batch Export (IRremote) - RAW

unsigned int Signal_0_4[] = {3400,1700,432,1298,432,1298,432,432,432,432,432,432,432,1298,432,432,432,432,432,1298,432,1298,432,432,432,1298,432,432,432,432,432,1298,432,1298,432,432,432,1298,432,1298,432,432,432,432,432,1298,432,432,432,432,432,1298,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,1298,432,432,432,432,432,432,432,432,432,432,432,1298,432,432,432,432,432,432,432,432,432,1298,432,1298,432,1298,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,1298,432,1298,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,1298,432,432,432,1298,432,1298,432,432,432,1298,432,1298,432,432,432,1298,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,1298,432,1298,432,1298,432,1298,432,432,432,432,432,432,432,432,432,16383,3400,1700,432,1298,432,1298,432,432,432,432,432,432,432,1298,432,432,432,432,432,1298,432,1298,432,432,432,1298,432,432,432,432,432,1298,432,1298,432,432,432,1298,432,1298,432,432,432,432,432,1298,432,432,432,432,432,1298,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,1298,432,432,432,432,432,432,432,432,432,432,432,1298,432,432,432,432,432,432,432,432,432,1298,432,1298,432,1298,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,1298,432,1298,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,1298,432,432,432,1298,432,1298,432,432,432,1298,432,1298,432,432,432,1298,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,1298,432,1298,432,1298,432,1298,432,432,432,432,432,432,432,432,432}; //AnalysIR Batch Export (IRremote) - RAW 
 
unsigned int Signal_0_0[] = {3472,1656,464,1248,480,1236,464,400,464,396,464,396,484,1228,452,408,456,404,480,1232,468,1244,492,372,456,1256,468,396,468,396,480,1232,468,1244,484,380,448,1260,456,1256,464,400,480,380,488,1232,464,388,480,384,484,1228,488,380,480,380,484,380,488,376,480,388,480,380,456,408,468,392,468,388,468,400,480,384,464,392,464,396,452,420,464,396,460,400,464,396,464,400,460,400,480,380,460,404,488,376,468,396,468,396,484,380,472,388,484,1228,492,376,456,408,480,384,480,380,464,400,464,1248,468,1244,464,400,464,396,484,384,464,392,480,380,484,380,408,448,456,408,488,376,484,1232,460,1256,484,384,464,400,464,396,484,384,464,392,484,376,468,396,464,396,464,1244,456,408,464,1248,464,1248,464,396,480,1232,456,408,452,1260,484,384,452,404,484,376,468,396,456,404,464,396,464,400,464,392,456,404,456,408,464,392,488,376,480,384,484,384,464,396,464,392,464,400,464,388,456,408,464,400,464,388,488,380,464,396,464,392,484,384,464,404,464,388,488,380,464,396,456,404,468,392,464,396,468,396,464,392,408,452,460,404,464,396,484,380,464,396,464,392,456,412,456,408,464,400,452,408,464,400,408,452,460,396,408,456,464,396,484,380,464,396,480,1232,456,1256,468,1248,480,1232,456,1256,484,380,468,1244,452,16904,3576,1684,464,1260,460,1252,464,396,464,396,464,400,464,1244,464,392,456,408,408,1304,464,1252,480,376,456,1260,464,392,456,420,408,1304,480,1236,464,396,464,1248,464,1248,460,396,456,404,456,1256,408,464,464,396,464,1248,464,392,484,380,480,384,464,392,452,416,452,408,484,388,464,392,484,380,480,384,480,388,452,404,408,452,456,412,452,408,464,396,464,396,464,396,464,400,464,396,464,392,408,456,464,400,408,448,484,384,464,396,480,1232,484,384,452,404,488,376,456,408,464,408,464,1248,464,1248,464,396,464,396,480,380,464,396,480,384,408,448,492,384,488,380,408,452,460,1248,484,1228,456,408,464,396,464,392,484,384,464,396,452,404,464,400,464,392,464,1252,464,396,408,1308,480,1236,464,400,408,1304,480,384,460,1252,464,392,488,380,464,392,492,380,488,376,452,408,484,380,460,404,464,396,480,384,464,392,460,400,464,396,464,400,452,404,408,452,456,412,452,408,464,396,464,396,464,396,464,400,464,396,464,392,408,456,464,400,408,448,460,408,464,396,480,380,464,396,464,396,464,396,464,396,464,396,408,448,412,452,464,396,464,400,484,380,464,400,452,408,408,452,404,456,408,456,484,376,456,408,480,384,464,392,464,400,408,456,408,1304,464,1252,464,1244,416,1300,460,1256,488,384,464,1248,464};//AnalysIR Batch Export RAW
 
void setup()
{
  Serial.begin(9600);
  irsend.enableIROut(khz);
  delay(1000);
  Serial.println(F("AnalysIR"));
}

void loop()
{

  irsend.sendRaw(Signal_0_0, sizeof(Signal_0_0)/sizeof(int), khz); //AnalysIR Batch Export (IRremote) - RAW
   // AnalysIR IR Protocol: MITSUBISHI288AC, Key:  

  delay(10000); //send signal every 10 seconds for testing
}
