int x, y = 0;
byte zona=10;
byte contatore_zona=0;

int readY() // returns the value of the touch screen's X-axis
{
  int xr = 0;
  pinMode(14, INPUT);   // A0
  pinMode(15, OUTPUT);    // A1
  pinMode(16, INPUT);   // A2
  pinMode(17, OUTPUT);   // A3
  digitalWrite(15, LOW); // set A1 to GND
  digitalWrite(17, HIGH);  // set A3 as 5V
  delay(5); // short delay is required to give the analog pins time to adjust to their new roles
  xr = analogRead(0); //  return xr;
}

int readX() // returns the value of the touch screen's Y-axis
{
  int yr = 0;
  pinMode(14, OUTPUT);   // A0
  pinMode(15, INPUT);    // A1
  pinMode(16, OUTPUT);   // A2
  pinMode(17, INPUT);   // A3
  digitalWrite(14, LOW); // set A0 to GND
  digitalWrite(16, HIGH);  // set A2 as 5V
  delay(5); // short delay is required to give the analog pins time to adjust to their new roles
  yr = analogRead(1); //
  return yr;
}

void lettura_tastiera(void)
{
  x = readX();
  y = readY();
  y = map(y, 75, 840, 0, 650);
  x = map(x, 100, 780, 0, 500);
  if ((x < 0) || (y < 0))//se il touch restituisce valori inferiori a 0 li azzero
  {
    x = 0;
    y = 0;
  }
if((x == 0) && (y == 0)){
  zona=10;
  numero_digitato=10;
return;
}
else
{
  //----------------------------------------------------------------------ZONE-------------------------------------------------------------------
  contatore_zona++;
  if ((y <= 162) && (x < 165)) // CASO ZONA 10
  {
    zona = 10; //
    //numero_digitato=1;
  }
  if ((y > 162) && (y <= 325) && (x < 165)) // CASO ZONA 7
  {
    zona = 7; //

  }
  if ((y > 325) && (y <= 487) && (x < 165)) // CASO ZONA 4
  {
    zona = 4; //
  }
  if (((y > 487) && y <= 650) && (x < 165)) // CASO ZONA 1
  {
    zona = 1; //
  }
  if ((y <= 162) && (x >= 165) && (x < 330)) // CASO ZONA 0
  {
    zona = 0; //
  }
  if ((y <= 162) && (x >= 330) && (x < 500)) // CASO ZONA 12
  {
    zona = 12; //
  }
  if ((y > 162) && (y <= 325) && (x >= 165) && (x < 330)) // CASO ZONA 8
  {
    zona = 8; //
  }
  if ((y > 162) && (y <= 325) && (x >= 330) && (x < 500)) // CASO ZONA 9
  {
    zona = 9; //
  }
  if ((y > 325) && (y <= 487) && (x >= 165) && (x < 330)) // CASO ZONA 5
  {
    zona = 5; //
  }
  if ((y > 325) && (y <= 487) && (x >= 330) && (x < 500)) // CASO ZONA 6
  {
    zona = 6; //
  }
  if ((y > 487) && (y <= 650) && (x >= 165) && (x < 330)) // CASO ZONA 2
  {
    zona = 2; //
  }
  if ((y > 487) && (y <= 650) && (x >= 330) && (x < 500)) // CASO ZONA 3
  {
    zona = 3; //
  }

  if(contatore_zona<=5) return;
 // 
}
 delay(10);

// Serial.println(zona);
 //if((x == 0) && (y == 0))return;//se il touch non riceve tocchi allora lo rileggo
 //else numero_digitato = zona;//zona equivale al numero digitato
 contatore_zona=0;
 numero_digitato = zona;
 zona=10;
Serial.print("numero_digitato=");
 Serial.println(numero_digitato);

}
