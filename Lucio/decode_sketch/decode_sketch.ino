
#define IN_PIN 2
#define MAX_CAPTURE 300

//globals
uint32_t now, start, capture[MAX_CAPTURE];
volatile bool flag_complete;
uint16_t i;
int first,second,diff,total;
int counter=0;
int bits=0;
unsigned long data=0;
unsigned long data1=0;
unsigned long data2=0;
unsigned long data3=0;
unsigned long data4=0;
unsigned long data5=0;

//interrupt fires on ir event (rise/fall)
void IRInterrupt(void) {
  now = micros();
  capture[i++] = now - start;
  start = now;
  if (i >= MAX_CAPTURE) {
    detachInterrupt(0);
    flag_complete = true;
  }
}

void setup(void) {
  flag_complete = false;
  start = 0;
  now = 0;
  i = 0;
  Serial.begin(9600);
  attachInterrupt(0, IRInterrupt, CHANGE);
  Serial.println("Ready to capture.");
}

void loop(void) {
  while (1) {
    if (flag_complete) {
      for (i=0; i < MAX_CAPTURE; i++) {   
        counter++;  
        first=capture[i];
        second=capture[i+1];

        if(first>12900 && first<13700){
          break;
        }
  if(counter<2)
  {

          total=first+second;

          if(total<900 && total>700)
          {
            data <<= 1;
          }
          if(total>1600 && total<1800)
          {
            data = (data << 1) | 1;  
          }
          bits++;
  }
  else
  {
          counter=0;
  }
        if(bits==34)
        {
          data1=data;
        }
        else if(bits==66)
        {
          data2=data;
        }
        else if(bits==98)
        {
          data3=data;
        }
        else if(bits==130)
        {
          data5=data;
        }
        else if(bits==146)
        {
          data5=data;
          Serial.print(data1,HEX);
          Serial.print("000");
          Serial.print(data2,HEX);
          Serial.print(data3,HEX);
          Serial.print(data4,HEX);
          Serial.print("0000");
          Serial.println(data5,HEX);
        }

        flag_complete = false;
      }
    }
  }
}

