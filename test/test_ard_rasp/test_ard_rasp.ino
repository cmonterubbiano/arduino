int ix;
void setup(){
  Serial.begin(9600);
}
 
void loop(){
  ix++;
  
  Serial.print("Ciao Pi\t");
  Serial.println(ix);
  delay(2000);
}                                                                     
