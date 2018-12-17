/*
Programmatore: EletroLaix
Programma:     Controllo arduino Wi-Fi
Versione:      3.5
Video: http://www.youtube.com/playlist?list=PLA56A165F3070591F
*/

char T[]= {'r','v','b','i'};
int  D[]= {  0,  0,  0,  0};
int  P[]= {  3,  5,  6, 11,  4};
int  N[]= {0,0,0};
int  e;
int  f;
char d;

void setup() {
  Serial.begin(9600);
  for (int i=0;i<=4;i+=1){pinMode(P[i], OUTPUT);}
  pinMode(A0,INPUT);
}

void loop() {
 for (int i=0;i<=4;i+=1) {analogWrite(P[i],D[i]);}
 if (Serial.available()<=0){Invio();}
}


void serialEvent(){
  switch (e) {
    
    case 0:
    d = Serial.read();
    for (int i=0;i<=4;i+=1) { if ( d == T[i] ) {e = 1; f=i;} }
    break;
    
    case 1:
    N[0] = Serial.read();
    e = 2;
    break;
    
    case 2:
    N[1] = Serial.read();
    e = 3;
    break;
    
    case 3:
    N[2] = Serial.read();
    e = 0;
    D[f]= ((N[0]-48)*100)+((N[1]-48)*10)+(N[2]-48);
    break;
  }
}

void Invio(){
 int lu = analogRead(A0);
 Serial.print ("l");
 Serial.print (lu/100);
 Serial.print ((lu/10)-((lu/100)*10));
 Serial.print (lu-((lu/10)*10));
}
