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
int temp;
int humi;
bool sensio; // sensor in ordnung i.o.
int senstimer;
int wlantimer;
bool wlanstate;

WebServer server(80);

TaskHandle_t Core0TaskHnd;
TaskHandle_t Core1TaskHnd;

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
  sprintf(buf, "<temp id=\"1\">%d°</temp>", temp);
  strcat(XML, buf);
  sprintf(buf, "<humi id=\"2\">%d"
               "%"
               "</humi>",
          humi);
  strcat(XML, buf);

  if (sensio = true)
  {
    strcat(XML, "<sensio id=\"3\">1</sensio>");
  }
  else if (sensio = false)
  {
    strcat(XML, "<sensio id=\"3\">0</sensio>");
  }

  strcat(XML, "</Data>");
  server.send(200, "text/xml", XML);
  Serial.println("Sending XML");
  Serial.println(XML);
}

void CoreTask0(void *parameter)
{
  yield();
  for (;;)
  {
    Serial.println("1");
    yield();
    temp = sens.readTemperature(); // Temp auslesen
    yield();
    humi = sens.readHumidity(); // humidity auslesen
    yield();
    Serial.println("2");
    if (isnan(humi) || isnan(temp))
    {
      Serial.println("Fehler beim auslesen des Sensors!");
      sensio = false;
      Serial.println("3");
    }
    else
    {
      sensio = true;
      Serial.println("4");
    }
    yield();
    Serial.println("Core0");
    yield();
    delay(1000);
  }
}

void setup()
{
  Serial.begin(9600);

  // Watchdogtimer deaktivieren
  disableCore0WDT(); // core 0
  // disableCore1WDT(); // core 1

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
  xTaskCreatePinnedToCore(CoreTask0, "CPU_0", 1000, NULL, 1, &Core0TaskHnd, 0);
}

void loop()
{
  server.handleClient();
}