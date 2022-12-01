#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>
#include "index.h"
#include "functions.h"
#include "SoftwareSerial.h"
#include "MHZ19.h"
#include "XML.h"

char LOCAL_SSID[] = "FBI VAN";
char LOCAL_PASS[] = "12345678.";

DHT sens(27, DHT11);

char XML[500];
char buf[200];
int temp;
int humi;
bool sensio; // sensor in ordnung i.o.
int senstimer;
int wlantimer;
bool wlanstate;
int co2;

WebServer server(80);

#define RX_PIN 17
#define TX_PIN 16
#define BAUDRATE 9600 // Native to the sensor (do not change)

MHZ19 myMHZ19;
SoftwareSerial mySerial(RX_PIN, TX_PIN); // Uno example

void Sens()
{
    senstimer = millis() + 30000;

    temp = sens.readTemperature(); // Temp auslesen
    humi = sens.readHumidity();    // humidity auslesen
    co2 = myMHZ19.getBackgroundCO2();
    //==========================================================

    // error handling
    if (isnan(humi) || isnan(temp))
    {
        Serial.println("Fehler beim auslesen des Sensors!");
        sensio = false;
        return;
    }
    else
    {
        sensio = true;
        return;
    }
}

void SendWebsite()
{
    Serial.println("new client, sending web page");
    server.send(200, "text/html", PAGE_MAIN);
    Serial.println("Website send");
}

void SendXML()
{
    Sens();
    Serial.println("XML requested");
    sprintf(XML, XML_template, temp, humi, sensio, co2);
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

    mySerial.begin(BAUDRATE); // Uno example: Begin Stream with MHZ19 baudrate
    myMHZ19.begin(mySerial);  // *Important, Pass your Stream reference here

    //===========================================================

    // connect to wifi
    ConnectWlan(LOCAL_PASS, LOCAL_SSID);
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

    wlantimer = millis() + 5000;
    if (WiFi.status() != WL_CONNECTED)
    {
        ConnectWlan(LOCAL_PASS, LOCAL_SSID);
    }

    //===========================================================
    server.handleClient();
    //  Serial.println("Loop");
}