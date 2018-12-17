/*
 * Prova di colloquio tramite porta seriale
AT+CGMM 

SIMCOM_SIM900

OK
AT

OK
AT+CGMI 

SIMCOM_Ltd

OK
Cambio velocita di trasmissione
AT+IPR=9600

OK

ATD337634273;
ATD0733816431;
ATD3312229982;
Chiamate a numeri
ATD##002#; // disattivazione segreteria vodafone

OK


+CUSD: 2

*/



#include <SoftwareSerial.h>

SoftwareSerial GPRS(7, 8); //Seriale simulata per comunicare col modem

unsigned char buffer[64]; // buffer per i dati scambiati sulla seriale
int count=0;              // contatore dati nel buffer
int flag =0;
String  inputString = "";
boolean  stringComplete = false;   

void setup()
{
  GPRS.begin(9600);   // Baudrate di default del modem SIM900
  Serial.begin(9600); // Baudrate della seriale di Arduino 
//  Serial.println("Powering Up SIM900");
//  pinMode(9, OUTPUT);
//  digitalWrite(9,LOW);
//  delay(100);
//  digitalWrite(9,HIGH);
//  delay(500);
//  digitalWrite(9,LOW);
//  delay(100);
//  Serial.println("SIM900 Powered Up");
 inputString.reserve(100);
}

void loop()
{
	if (GPRS.available())  // Se ci sono dati sulla seriale simulata (il modem ci invia dati)
	{
		while(GPRS.available()) // Finché ci sono dati disponibili
		{
			buffer[count++]=GPRS.read(); // Inserisce i byte nel buffer
			if(count == 64)break;
		}
    
		Serial.write(buffer,count);  // Scrive i dati ricevuti sulla seriale di Arduino
		delay(200);
		clearBufferArray();  // Svuota il buffer
		count = 0;           
	}

	serialEvent(); //call the function

	if (stringComplete)
	{
		int ix;

		for(ix =0; ix <inputString.length(); ix++)
		{
			flag =1;
			GPRS.write(inputString[ix]); // Inviamo i dati al modem
			delay(200);
		}
		inputString = "";
		stringComplete = false;
	}
}
void clearBufferArray() // Funzione di supporto per svuotare il buffer
{
	char  appoggio[100];

	*appoggio ='\0';
  
	for (int i=0; i<count;i++)
	{
		char  appo;
    
		if (!flag && isPrintable(buffer[i]))
		{
			char  appo =buffer[i];
			sprintf(&appoggio[strlen(appoggio)], "%c", appo);
		}
		buffer[i]=NULL;
	}
	flag =0;
	if (strlen(appoggio) && !strncmp(appoggio, "RING+CLIP:", 10))
	{
		int	virgola =0;
		int	ultimo_apice =0;
		int	penultimo_apice =0;
		int	ix =(strlen(appoggio) -1);
		
		for (;ix ; ix--)
		{
			if (appoggio[ix] ==',' && !virgola)
			{
				virgola =ix;
				continue;
			}
			if (appoggio[ix] =='"' && virgola)
			{
				if (!ultimo_apice)
				{
					ultimo_apice =ix;
					continue;	
				}
				penultimo_apice =ix;
				break;
			}
		}
		if (virgola && ultimo_apice && penultimo_apice)
		{
			if ((virgola -ultimo_apice) ==1
			&& (ultimo_apice -penultimo_apice) !=1)
			{
				int	fine =13;
				char *chiudi ="ATH";
				char *cancello = "ATD00393312229982\;";
				int	ix;
				
//				for(ix =0; chiudi[ix]; ix++)
//				{
//					GPRS.write(chiudi[ix]); // Inviamo i dati al modem
//					delay(200);
//				}
//				GPRS.write(fine); // Inviamo i dati al modem
        GPRS.println(chiudi);
				Serial.flush();
				delay(3000);
//				for(ix =0; cancello[ix]; ix++)
//				{
//					GPRS.write(cancello[ix]); // Inviamo i dati al modem
//					delay(200);
//				}
//				GPRS.write(fine); // Inviamo i dati al modem
        GPRS.println(cancello);
			}
			// else
			// {
					// Serial.print("p_ap -> ");
					// Serial.print(penultimo_apice);
					// Serial.print(" u_ap -> ");
					// Serial.print(ultimo_apice);
					// Serial.print(" virg -> ");
					// Serial.print(virgola);
			// }
		}
		// Serial.println(" ");
		// Serial.print("appoggio -->");
		// Serial.println(appoggio);                 
	}
 }
void serialEvent()
{
  while (Serial.available())
  {
    // get the new byte:
    char inChar = (char)Serial.read();
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n' || inChar == '\r')
    {
      inputString += inChar;
      inputString += "";
      stringComplete = true;
    }
    else  inputString += inChar;  // add it to the inputString:
  }
} 

