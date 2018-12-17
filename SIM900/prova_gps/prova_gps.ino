#include "SIM900.h"
#include <SoftwareSerial.h>
/**********************************codice aggiunto******************/
#include "inetGSM.h"
/******************************fine codice aggiunto******************/
//#include "sms.h"
//#include "call.h"
#include "gps.h"
#include "stdlib.h"

/**********************************codice aggiunto******************/
InetGSM inet;
/********************************fine codice aggiunto******************/

//CallGSM call;
//SMSGSM sms;

GPSGSM gps;

/**********************************codice aggiunto******************/
char msg[50];
int numdata;
char lat_str[10];
char long_str[10];
//char inSerial[50];
//int i=0;
//boolean started=false;
/********************************fine codice aggiunto******************/

char lon[15];
float lon_db;
char lat[15];
float lat_db;
char alt[15];
char time[20];
char vel[15];
char msg1[5];
char msg2[5];

char stat;
char inSerial[20];
int i=0;
boolean started=false;

void setup() 
{
  //Serial connection.
  Serial.begin(9600);
  Serial.println("GSM Shield testing.");
  //Start configuration of shield with baudrate.
  //For http uses is raccomanded to use 4800 or slower.
  if (gsm.begin(4800)){
    Serial.println("\nstatus=READY");
    gsm.forceON();  //To ensure that SIM908 is not only in charge mode
    started=true;  
  }
  else Serial.println("\nstatus=IDLE");
  
  if(started){
    //GPS attach
    if (gps.attachGPS())
      Serial.println("status=GPSREADY");
    else Serial.println("status=ERROR");
  
    delay(20000); //Time for fixing
    stat=gps.getStat();
  if(stat==1)
    Serial.println("NOT FIXED");
  else if(stat==0)
    Serial.println("GPS OFF");
  else if(stat==2)
    Serial.println("2D FIXED");
  else if(stat==3)
    Serial.println("3D FIXED");
  delay(5000);
  //Get data from GPS
  gps.getPar(lon,lat,alt,time,vel);
  Serial.println(lon);
  Serial.println(lat);
  Serial.println(alt);
  Serial.println(time);
  Serial.println(vel);
/***************************codice aggiunto***********************/ 
  lon_db=convert(lon);
        lat_db=convert(lat);
  sprintf(long_str,"%f",lon_db);
  sprintf(lat_str,"%f",lat_db);


//GPRS attach, put in order APN, username and password.
    //If no needed auth let them blank.
  // if (inet.attachGPRS("internet.wind", "", ""))
    if (inet.attachGPRS("wap.omnitel.it", "", ""))
      Serial.println("status=ATTACHED");
    else Serial.println("status=ERROR");
    delay(1000);
    
    //Read IP address.
    gsm.SimpleWriteln("AT+CIFSR");
    delay(5000);
    //Read until serial buffer is empty.
    gsm.WhileSimpleRead();
  
    //TCP Client GET, send a GET request to the server and
    //save the reply.
    numdata=inet.httpPOST("www.spaziopixel.com", 80, "/coordinate_arduino/index.php", "lat=lat_str&long=lon_str", msg, 50);
    //Print the results.
    Serial.println("\nNumber of data received:");
    Serial.println(numdata);  
    Serial.println("\nData received:"); 
    Serial.println(msg);
/********************************fine codice aggiunto*****************/
  }
};

void loop() 
{
  //Read for new byte on serial hardware,
  //and write them on NewSoftSerial.
  serialhwread();
  //Read for new byte on NewSoftSerial.
  serialswread();
};

void serialhwread(){
  i=0;
  if (Serial.available() > 0){            
    while (Serial.available() > 0) {
      inSerial[i]=(Serial.read());
      delay(10);
      i++;      
    }
    inSerial[i]='\0';
    if(!strcmp(inSerial,"/END")){
      Serial.println("_");
      inSerial[0]=0x1a;
      inSerial[1]='\0';
      gsm.SimpleWriteln(inSerial);
    }
    //Send a saved AT command using serial port.
    if(!strcmp(inSerial,"GPS")){
        stat=gps.getStat();
  if(stat==1)
    Serial.println("NOT FIXED");
  else if(stat==0)
    Serial.println("GPS OFF");
  else if(stat==2)
    Serial.println("2D FIXED");
  else if(stat==3)
    Serial.println("3D FIXED");

delay(5000);
//Get data from GPS
gps.getPar(lon,lat,alt,time,vel);
lon_db=convert(lon);
lat_db=convert(lat);
Serial.println("dd.dddddd data:");
Serial.print("LON: ");
Serial.println(lon_db,4);
Serial.print("LAT: ");
Serial.println(lat_db,4);


    }
    //Read last message saved.
    if(!strcmp(inSerial,"MSG")){
      Serial.println(msg1);
    }
    else{
      Serial.println(inSerial);
      gsm.SimpleWriteln(inSerial);
    }    
    inSerial[0]='\0';
  }
}

void serialswread(){
  gsm.SimpleRead();
}

int strpos(char *str, char *target) {
char *res=0;
res = strstr(str, target);
if (res == NULL) return false;
else return res - str;
}

float convert(char* str){
float mmmmmm;
float dd;
int pos;
pos=strpos(str,".");
char dd_str[pos-1];
dd_str[0]='\0';
char mmmmmm_str[6];
mmmmmm_str[0]='\0';
for (int i=0; i<pos-2; i++){
dd_str[i]=str[i];
}
dd_str[pos-2]='\0';
dd=atof(dd_str);
mmmmmm_str[0]=str[pos-2];
mmmmmm_str[1]=str[pos-1];
mmmmmm_str[2]=str[pos];
mmmmmm_str[3]=str[pos+1];
mmmmmm_str[4]=str[pos+2];
mmmmmm_str[5]=str[pos+3];
mmmmmm_str[6]='\0';
mmmmmm=atof(mmmmmm_str);
float result;
result=dd+mmmmmm/60;
return result;
}
