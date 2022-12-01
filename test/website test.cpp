#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>
#include "index.h"
#include "functions.h"

#define LOCAL_SSID "FBI VAN"
#define LOCAL_PASS "12345678."

DHT sens(27, DHT11);

char XML[500];
char buf[200];
int temp = 100;
int humi = 100;
bool sensio = 1; // sensor in ordnung i.o.
int senstimer;
int wlantimer;
bool wlanstate;

WebServer server(80);

void SendWebsite()
{
  Serial.println("new client, sending web page");
  server.send(200, "text/html", PAGE_MAIN);
  Serial.println("Website send");
}

void SendXML()
{
  Serial.println("XML requested");
  strcpy(XML, "<?xml version = '1.0'?><Data>");
  sprintf(buf, "<temp>%d</temp>", temp);
  strcat(XML, buf);
  sprintf(buf, "<humi id=\"2\">%d</humi>", temp);
  strcat(XML, buf);

  strcat(XML, "<sensio id=\"3\">1</sensio>");

  strcat(XML, "</Data>");
  server.send(200, "text/xml", XML);
  Serial.println("Sending XML");
  Serial.println(XML);
}

void setup()
{
  Serial.begin(9600);
  // Watchdogtimer deaktivieren
  disableCore0WDT(); // core 0
  disableCore1WDT(); // core 1

  sens.begin(); // DHT sensor initialisieren

  //===========================================================

  // connect to wifi
  Serial.println("Connecting to Wifi");
  WiFi.begin(LOCAL_SSID, LOCAL_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected");
  //===========================================================

  Serial.println("Starting Server");
  server.on("/", SendWebsite);
  server.on("/xml", SendXML);

  server.begin();

  printWifiStatus(); // look in functions
  Serial.println("Server started");
}

void loop()
{

  if (millis() > wlantimer)
  {
    wlantimer = millis() + 1000;
    if (WiFi.status() == WL_CONNECTED)
    {
      return;
    }
    else
    {
      Serial.println("Wlan disconnected");

      Serial.println("Connecting to Wifi");
      WiFi.begin(LOCAL_SSID, LOCAL_PASS);
      while (WiFi.status() != WL_CONNECTED)
      {
        delay(500);
        Serial.print(".");
      }
      Serial.println("Connected");
    }
  }

  //===========================================================}
  server.handleClient();
  //  Serial.println("Loop");
}