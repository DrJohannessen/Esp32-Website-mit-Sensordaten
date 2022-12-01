#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

void printWifiStatus()
{ // prints all important information about Wifi

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("Open http://");
  Serial.println(WiFi.localIP());
}

void ConnectWlan(char PASS[], char RSSI[])
{
  Serial.println("Connecting to Wifi");
  WiFi.begin(RSSI, PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected");
}