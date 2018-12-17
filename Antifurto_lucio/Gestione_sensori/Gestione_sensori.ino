/*
  Simple example for receiving
  
  http://code.google.com/p/rc-switch/
*/

#include <RCSwitch.h>
#include <EEPROM.h>

RCSwitch mySwitch = RCSwitch();

char inputString[200];         // a String to hold incoming data
boolean stringComplete = false;  // whether the string is complete
long	codice433 =0L;

void elenca_codici()
{
	int	ix =0;
	long numero =0L;
	int	volte =1;
	int address = 0;
	byte value;
	
		Serial.print("Prog.");
		Serial.print("\t");
		Serial.print("Stanza");
		Serial.print("\t");
		Serial.print("Codice");
		Serial.print("\t");
		Serial.print("\t");
		Serial.println("Descrizione");
  
	while(1)
	{
		
		for (ix =0; ix <2 && (address < EEPROM.length()); ix++, address++)
		{
			value = EEPROM.read(address);
			numero =numero * 10  +(value -48);
		}
		if (numero <0 || numero >99)
			break;
		
		Serial.print(volte);
		Serial.print("\t");
		Serial.print(numero);
		for (ix =0, numero =0L; ix <8 && (address < EEPROM.length()); ix++, address++)
		{
			value = EEPROM.read(address);
			numero =numero * 10  +(value -48);
		}
		Serial.print("\t");
		if (numero !=99999999L)
			Serial.print(numero);
		else
			Serial.print("Annullato");
		Serial.print("\t");
		Serial.print("\t");
		for (ix =0, numero =0L; ix <20 && (address < EEPROM.length()); ix++, address++)
		{
			value = EEPROM.read(address);
			char ch =char(value);
			Serial.print(ch);
		}
		Serial.println(" ");

		volte++;
		numero =0L;
		delay(5);
	}
	Serial.println(" ");
	Serial.println("L [Lista codici inseriti]");
	Serial.println("A,Codice stanza [Annullare codice]");
	Serial.println("I,Codice stanza,Descrizione,Codice/X [Inserire/Modificare]");
	Serial.println("O [Ordinamento stanze]");
}

void intercetta_codici()
{
	if (mySwitch.available())
	{
    
		int value = mySwitch.getReceivedValue();
    
		if (value == 0)
		{
			Serial.print("Unknown encoding");
		}
		else
		{
			Serial.print("Received ");
			codice433 =mySwitch.getReceivedValue();
			Serial.print(codice433);
			Serial.print(" / ");
			Serial.print( mySwitch.getReceivedBitlength() );
			Serial.print("bit ");
			Serial.print("Protocol: ");
			Serial.println( mySwitch.getReceivedProtocol() );
		}

		mySwitch.resetAvailable();
	}
}

void	annulla_codice()
{
	int	ix;
	int	numero;
	int address = 0;
	
	if (inputString[1] !=',')
	{
		Serial.println("Sintassi errata il secondo carattere deve essere \",\"");
		return;
	}
	for(ix =2; ix <strlen(inputString); ix++)
	{
		if (inputString[ix] <'0'
		|| inputString[ix] >'9')
			continue;
		numero++;
	}
	if (numero ==(strlen(inputString) -2) && numero <3)
	{
		char	appo[3];
		
		sprintf(appo, "%02d", atoi(&inputString[2]));
		if (!atoi(appo))
			Serial.println("Dopo la virgola valore diverso da zero");
		else
		{
			numero =0;
			
			while(1)
			{
				byte value;
				
				for (ix =0; ix <2 && (address < EEPROM.length()); ix++, address++)
				{
					value = EEPROM.read(address);
					numero =numero * 10  +(value -48);
				}
				if (numero <1 || numero >99)
					break;
				if (numero !=atoi(appo))
				{
					address +=28;
					numero =0;
					continue;
				}
				for (ix =0, numero =0L; ix <8 && (address < EEPROM.length()); ix++, address++)
					EEPROM.write(address, '9');

				for (ix =0, numero =0L; ix <20 && (address < EEPROM.length()); ix++, address++)
				{
					byte by =byte(' ');
					EEPROM.write(address, by);
				}
				break;
			}
		}
	}
	else
		Serial.println("Dopo la virgola sono ammessi solo numeri tra 1 e 99");
}

void	ins_mod_codice()
{
	int	ix;
	int	numero;
	int address = 0;
	int	numerovirgole;
	
	if (inputString[1] !=',')
	{
		Serial.println("Sintassi errata il secondo carattere deve essere \",\"");
		return;
	}
	for(ix =2, numerovirgole =0; ix <strlen(inputString); ix++)
	{
		if (inputString[ix] ==',')
			numerovirgole++;
	}
	if (numerovirgole !=2)
	{
		Serial.print(numerovirgole);
		Serial.print(" - ");
		Serial.println("Sintassi errata, numero virgole ERRATO");
		return;
	}
	for(ix =2; ix <strlen(inputString) && inputString[ix] !=',' ; ix++)
	{
		char	pippo[2];
		
		sprintf(pippo, "%c", inputString[ix]);
		// if (inputString[ix] ==',')
			// break;

		if (inputString[ix] <'0'
		|| inputString[ix] >'9')
			continue;
		numero++;
	}
	if (ix <strlen(inputString)
	&& (numero ==(ix -2)) && numero <3)
	{
		char	appo[3];
		
		sprintf(appo, "%d", atoi(&inputString[2]));
		if (!atoi(appo))
			Serial.println("Dopo la virgola valore diverso da zero");
		else
		{
			char	stanza[3];
			char	descrizione[21];
			
			numero =0;
			sprintf(stanza, "%02d", atoi(appo));
			sprintf(descrizione, "%-20.20s", "");
			
			while(1)
			{
				byte	value;
				int		iy;
			
				
				for (iy =0; iy <2 && (address < EEPROM.length()); iy++, address++)
				{
					value = EEPROM.read(address);
					numero =numero * 10  +(value -48);
				}
				if (numero >0 && numero <100 && numero !=atoi(appo))
				{
					address +=28;
					numero =0;
					continue;
				}

				for (ix +=1, iy =0;; ix++, iy++)
				{
					if (inputString[ix] ==',')
						break;
					if (iy <20)
						descrizione[iy] =inputString[ix];
				}
				
				codice433 =0L;
				if (inputString[(++ix)] =='X')
				{
					long	tempo =millis();
					
					Serial.println("ATTESA DEL CODICE");
					while(!codice433)
					{
						intercetta_codici();
						if ((millis() -tempo) >20000L)
						{
							Serial.println("TEMPO DI ATTESA SCADUTO");
							break;
						}
					}
				}
				else
				{
					int	iy;
					char	input433[9];
					
					*input433 ='\0';
					
					for(iy =0; ix <strlen(inputString) && iy <8; ix++, iy++)
					{
						if (inputString[ix] <'0'
						|| inputString[ix] >'9')
							break;
						sprintf(&input433[strlen(input433)], "%c", inputString[ix]);
					}
					if (ix <strlen(input433))
						codice433 =0L;
					else
						codice433 =atol(input433);
				}
				if (codice433)
				{
					byte by;
					char	appo[9];
					
					sprintf(appo, "%08ld", codice433);
					
					address -=2;

					for (ix =0; ix <strlen(stanza); ix++, address++)
					{
						byte by =stanza[ix];
						EEPROM.write(address, by);
					}
					for (ix =0; ix <strlen(appo); ix++, address++)
					{
						byte by =appo[ix];
						EEPROM.write(address, by);
					}
					for (ix =0; ix <strlen(descrizione); ix++, address++)
					{
						byte by =descrizione[ix];
						EEPROM.write(address, by);
					}
				}
				else
					Serial.println("Codice errato");
				break;
			}
		}
	}
	else
	{
		Serial.print(ix);
		Serial.print(" - ");
		Serial.println(numero);
		Serial.println("Dopo la virgola sono ammessi solo numeri tra 1 e 99 seguiti da \",\"");
	}
}

void	ordina_codice()
{
	int	appo_stanza =0;
	int	appo_stapre;
	char appo_record[31];
	char appo_recpre[31];
	int	scambio =1;
	int	ix;
	int	numero;
	int address = 0;
	
	while(scambio)
	{
		scambio =0;
		
		for (address =0; (address < EEPROM.length());)
		{
			byte value;

			for (ix =numero =0; ix <2; ix++)
			{
				value = EEPROM.read((address +ix));
				numero =numero * 10  +(value -48);
			}

			if (numero <1 || numero >99)
				break;
			
			for (ix =0; ix <30; ix++)
				appo_record[ix] = EEPROM.read((address +ix));
			appo_record[ix] ='\0';
			
			if (!address || numero >appo_stapre)
			{
				appo_stapre =numero;
				strcpy(appo_recpre, appo_record);
				address +=30;
				continue;
			}

			for (ix =0; ix <30; ix++)
				EEPROM.write((address +ix), appo_recpre[ix]);
			for (ix =0; ix <30; ix++)
				EEPROM.write((address -30 +ix), appo_record[ix]);
			scambio =1;
			address +=30;
		}
	}
	elenca_codici();
}

void setup()
{
	int	ix =0;
	long numero =0L;
	int	volte =1;
	// reserve 200 bytes for the inputString:
	*inputString= '\0';
	
	Serial.begin(9600);
	mySwitch.enableReceive(0);  // Receiver on inerrupt 0 => that is pin #2
	elenca_codici();
}

void loop()
{
// print the string when a newline arrives:
	if (stringComplete)
	{
		if (strlen(inputString))
		{
			// int	ix;
			
			// for (ix =0; ix <strlen(inputString); ix++)
			// {
				// byte by =byte(inputString[ix]);
				// Serial.print(by);
				// Serial.print("  <--Byte ");
				// Serial.print(inputString[ix], DEC);
				// char ch =char(by);
				// Serial.print("  Char--> ");
				// Serial.println(ch);
				
				// delay(10);
			// }
			
			Serial.print("Linea comando --> ");			
			Serial.println(inputString);
			if (toupper(*inputString) =='L')
				elenca_codici();
			else	if (toupper(*inputString) =='A')
				annulla_codice();
			else	if (toupper(*inputString) =='I')
				ins_mod_codice();
			else	if (toupper(*inputString) =='O')
				ordina_codice();
			else	Serial.println("Comando errato");
				
		}
		// clear the string:
		*inputString= '\0';
		stringComplete = false;
	}
}

void serialEvent()
{
	while (Serial.available())
	{
    // get the new byte:
		char inChar = (char)Serial.read();


    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
		if (inChar == '\n' || inChar =='\r')
		{
			stringComplete = true;
			// Serial.print("return -> ");
			// Serial.println(inputString);
		}
		else
		{
			sprintf(&inputString[strlen(inputString)], "%c", inChar);
//			Serial.println(inputString);
		}
	}
}
