int  led1 =3;
int  led2 =5;
int  led4 =6;

void setup()
{
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led4, OUTPUT);
 
  //init seriale
  Serial.begin(9600);
}
 
void loop() 
{
  int  num =random(0,7);
  Serial.print(num);
  switch(num)
  {
    case 0:
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led4, LOW);
      break;
    case 1:
      digitalWrite(led1, HIGH);
      digitalWrite(led2, LOW);
      digitalWrite(led4, LOW);
      break;
    case 2:
      digitalWrite(led1, LOW);
      digitalWrite(led2, HIGH);
      digitalWrite(led4, LOW);
      break;
    case 3:
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led4, LOW);
      break;
    case 4:
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led4, HIGH);
      break;
    case 5: 
      digitalWrite(led1, HIGH);
      digitalWrite(led2, LOW);
      digitalWrite(led4, HIGH);
      break;
     case 6: 
      digitalWrite(led1, LOW);
      digitalWrite(led2, HIGH);
      digitalWrite(led4, HIGH);
      break;
     case 7: 
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led4, HIGH);
      break;     
    default:
      break;
  }
   num =random(5, 200);
   Serial.print("-");
   Serial.println(num);
   delay((num *10));
}
