

// set pin numbers:
const int button1 = 5;     // the number of the pushbutton pin
const int button2 = 6;     // the number of the pushbutton pin
const int button3 = 7;     // the number of the pushbutton pin

const int rele1 =  8;      // the number of the LED pin
const int rele2 =  9;      // the number of the LED pin

// variables will change:
int rele1stato = 0;         // variable for reading the pushbutton status
int rele2stato = 0;         // variable for reading the pushbutton status
int letturaButton;

void setup() {
  // initialize the rele pin as an output:
  pinMode(rele1, OUTPUT);
  pinMode(rele2, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  
  digitalWrite(rele1, HIGH);
  digitalWrite(rele2, HIGH);
  Serial.begin(9600);
  Serial.println("Goodnight moon!");
}

void loop()
{
  // read the state of the pushbutton 1 value:
  letturaButton = digitalRead(button1);
  if (letturaButton == HIGH)
  {
    if (!rele1stato)
      digitalWrite(rele1, LOW);
    else
      digitalWrite(rele1, HIGH);
    if (!rele1stato)
      rele1stato =1;
    else
      rele1stato =0;
    Serial.print("Bottone1 ");
    Serial.print(rele1stato);
    Serial.print(" Rele2 ");
    Serial.println(rele2stato);
    delay(1000);
  }
  // read the state of the pushbutton 2 value:
  letturaButton = digitalRead(button2);
  if (letturaButton == HIGH)
  {
    if (!rele2stato)
      digitalWrite(rele2, LOW);
    else
      digitalWrite(rele2, HIGH);
    if (!rele2stato)
      rele2stato =1;
    else
      rele2stato =0;
    Serial.print("Bottone2 ");
    Serial.print(rele1stato);
    Serial.print(" Rele2 ");
    Serial.println(rele2stato);
    delay(1000);
  }
   // read the state of the pushbutton 1 value:
  letturaButton = digitalRead(button3);
  if (letturaButton == HIGH)
  {
    digitalWrite(rele1, HIGH);
    rele1stato =0;
    digitalWrite(rele2, HIGH);
    rele2stato =0;
    Serial.print("Bottone3 ");
    Serial.print(rele1stato);
    Serial.print(" Rele2 ");
    Serial.println(rele2stato);
    delay(1000);
  }
}
