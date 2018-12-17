#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
#define MIRF_PAYLOAD 32

byte rxData[MIRF_PAYLOAD + 1];
byte txData[MIRF_PAYLOAD + 1];
const char msg[]= "Nel mezzo del cammin di nostra vita mi ritrovai per una selva oscura, che' la diritta via era smarrita. Ahi quanto a dir qual era e' cosa dura, esta selva selvaggia e aspra e forte, che nel pensier rinova la paura! Tant'e' amara che poco e' piu' morte; ma per trattar del ben ch'i' vi trovai, diro' de l'altre cose ch'i' v'ho scorte. Io non so ben ridir com'i' v'intrai, tant'era pien di sonno a quel punto che la verace via abbandonai.";
int msgIndex = 0;
void setup()
{
  memset(rxData, 0, sizeof(rxData));
  memset(txData, 0, sizeof(txData));
  Serial.begin(9600);
  Mirf.cePin = 8;
  Mirf.csnPin = 7;
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte*)"clie1");
  Mirf.payload = MIRF_PAYLOAD;
  Mirf.channel = 10;
  Mirf.config();
}

void loop()
{
  Mirf.setTADDR((byte*)"serv1");
  memcpy(txData, msg + msgIndex, MIRF_PAYLOAD);
  Mirf.send(txData);
  while(Mirf.isSending())
  {
  }
  delay(10);
  unsigned long time = millis();
  while(!Mirf.dataReady())
  {
    if (( millis() - time ) > 1000)
    {
      Serial.println("timeout");
      return;
    }
  }
  Mirf.getData(rxData);
  Serial.println("RISPOSTA");
  msgIndex += MIRF_PAYLOAD;
 // if (msgIndex + MIRF_PAYLOAD > sizeof(msg))
  if (msgIndex  >= sizeof(msg))
  {
    msgIndex = 0;
    delay(1000);
  }
}
