#include <SPI.h>
#include <Ethercard.h>

// Enter a MAC address for your controller below.
byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
//An IP Address and Router settings just in case there is no DHCP available
IPAddress ip(192,168,1, 200);
IPAddress gateway(192,168,1, 1);
IPAddress subnet(255, 255, 255, 0);

//The Web Server we are connecting to
char serverName[] = "whatismyip.org";


// Initialize the Ethernet client library
EthernetClient client;

void setup() {
  // start the serial library:
  Serial.begin(9600);
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP, trying to set IP mannualy");
    Ethernet.begin(mac, ip, gateway, subnet);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");

  // if you get a connection, report back via serial:
  if (client.connect(serverName, 80)) {
    Serial.println("connected");
    // Make a HTTP request on the default page:
    client.println("GET / HTTP/1.0");
    client.println();
  } 
  else {
    // If you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}

void loop()
{
  // if there are incoming bytes available 
  // from the server, read them and print them:
  while (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    
    // do nothing forevermore:
    for(;;)
      ;
  }
}

