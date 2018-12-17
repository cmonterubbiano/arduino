// led in serie lato pin 9 con resistenza da 330 hom

int ledPin = 9;    // LED connesso al pin digitale 9

void setup() 
{
  Serial.begin(9600); 
} 

void loop()  
{ 
  for(int Val = 0 ; Val <= 255; Val +=25) // Vado da 0 a 255
  { 
    Serial.println(Val);
    analogWrite(ledPin, Val);  // imposto il valore sull'uscita digitale       
    delay(1000); // aspetto 10ms                           
  } 

  for(int Val = 255 ; Val >= 0; Val -=25) // ora scendo da 255 a 0
  { 
    Serial.println(Val);
    analogWrite(ledPin, Val);         
    delay(1000);                            
  } 
}
