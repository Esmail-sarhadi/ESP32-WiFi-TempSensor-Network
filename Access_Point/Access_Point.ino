#include <WiFi.h>
#include <WiFiUdp.h>

const char *ssid = "ESP_AP01";
const char *password = "ESP123456789";                                    //Choose a password thats at least 8 characters.

String postData = "Data received by Server.";
float numPeriod = 5000;                                                   //Check for number of stations.
long previousMillis = 0;

WiFiUDP Udp;
IPAddress localDevIP(192, 168, 6, 100);
IPAddress remoteDevIP(192, 168, 6, 101);
IPAddress gateway(192, 168, 6, 1);
IPAddress subnet(255, 255, 255, 0);
unsigned int localUdpPort = 61500;                                        //Local port to listen on.
unsigned int remoteUdpPort = 61501;
char incomingPacket[255];                                                 //Buffer for incoming packets.
char replyPacket[50];                                                     //A reply string to send back.
//*********************************************************************************************************
void setup()
{
  Serial.begin(115200);
  Serial.println();

  Serial.println(">> Activating soft-AP...");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(localDevIP, gateway, subnet);
  boolean result = WiFi.softAP(ssid, password);
  if (result == true)
  {
    Serial.println(">> Ready.");
  }
  else
  {
    Serial.println(">> Failed!");
  }
  Serial.print(">> Access point begins at: ");
  Serial.println(WiFi.softAPIP());

  Udp.begin(localUdpPort);
  Serial.printf(">> Now listening at IP %s, UDP port %d\n", WiFi.softAPIP().toString().c_str(), localUdpPort);
}
//*********************************************************************************************************
void loop()
{
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    //Receive incoming UDP packets.
    Serial.printf(">> Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.printf(">> UDP packet content: %s\n", incomingPacket);

    postData.toCharArray(replyPacket, 50);
    Udp.beginPacket(Udp.remoteIP().toString().c_str() , Udp.remotePort());
    Serial.print (">> Node IP: ");
    Serial.print (Udp.remoteIP().toString().c_str());
    Serial.print (" & Port: ");
    Serial.println (Udp.remotePort());
    Udp.print(replyPacket);
    Udp.endPacket();
    Serial.println(">> UDP process is done.");
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > numPeriod)
  {
    previousMillis = currentMillis;
    Serial.printf(">> Stations connected = %d\n", WiFi.softAPgetStationNum());
    Serial.println("********************************************************************");
  }
}
//*********************************************************************************************************
