/*
Was dieses Script macht:
-Sensordaten abrufen
-Sensordaten auf LCD anzeigen
-Website hosten
*/

#include <Arduino.h>
#include <WiFi.h>
#include <DHT.h>
#include "index.h"
#include "functions.h"
#include "SoftwareSerial.h"
#include "MHZ19.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

DHT sens(27, DHT11);

int temp;
int humi;
bool sensio; // sensor in ordnung i.o.
int co2;
int lcdTimer;

#define RXD2 16
#define TXD2 17
#define BAUDRATE 9600

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

void setup()
{
    Serial.begin(9600);
    // Watchdogtimer deaktivieren
    disableCore0WDT(); // core 0
    disableCore1WDT(); // core 1

    sens.begin(); // DHT sensor initialisieren
    Serial2.begin(BAUDRATE, SERIAL_8N1, RXD2, TXD2);
    myMHZ19.begin(Serial2); // *Important, Pass your Stream reference here

    // connect to wifi
    ConnectWlan(LOCAL_PASS, LOCAL_SSID);

    printWifiStatus(); // look in functions

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
}