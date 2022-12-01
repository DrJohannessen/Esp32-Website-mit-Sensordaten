#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>
#include "index.h"
#include "functions.h"
#include "SoftwareSerial.h"
#include "MHZ19.h"
#include "XML.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

char LOCAL_SSID[] = "NETGEAR-Guest";
char LOCAL_PASS[] = "alder,36";

DHT sens(27, DHT11);

char XML[500];
char buf[200];
int temp;
int humi;
bool sensio; // sensor in ordnung i.o.
int co2;
int lcdTimer;

WebServer server(80);

#define RXD2 16
#define TXD2 17
#define BAUDRATE 9600 // Native to the sensor (do not change)

MHZ19 myMHZ19;

LiquidCrystal_I2C lcd(0x27, 16, 2);

byte customChar[8] = {
    0b01100,
    0b10010,
    0b10010,
    0b01100,
    0b00000,
    0b00000,
    0b00000,
    0b00000};

void lcdWrite()
{
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(temp);
    lcd.write((byte)0);
    lcd.print("  H:");
    lcd.print(humi);
    lcd.print("%");
    lcd.setCursor(0, 1);
    lcd.print("CO2:");
    lcd.print(co2);
    lcd.print("ppm");
}

void SensRead()
{
    temp = sens.readTemperature(); // Temp auslesen
    humi = sens.readHumidity();    // humidity auslesen
    co2 = myMHZ19.getBackgroundCO2();
    temp = temp - 1;
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
    SensRead();
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
    Serial2.begin(BAUDRATE, SERIAL_8N1, RXD2, TXD2);
    myMHZ19.begin(Serial2); // *Important, Pass your Stream reference here

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
    lcd.init();
    lcd.backlight();
    lcd.createChar(0, customChar); // create a new custom character
}

void loop()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        ConnectWlan(LOCAL_PASS, LOCAL_SSID);
    }

    if (lcdTimer < millis())
    {
        lcdTimer = millis() + 300000;
        SensRead();
        lcdWrite();
        Serial.println("LCD");
    }

    //===========================================================
    server.handleClient();
    //  Serial.println("Loop");
}