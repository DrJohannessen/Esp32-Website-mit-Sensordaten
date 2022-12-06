/*
Was dieses Script macht:
-Sensordaten abrufen
-Sensordaten auf LCD anzeigen
- NPT Zeit auslesen
-Npt Zeit anzeigen auf LCD
-alle 10 sec updaten

=================
16:56--C:2000--
T:22*--H:20%---
=================
*/

#include <Arduino.h>
#include <WiFi.h>
#include <DHT.h>
#include "functions.h"
#include "SoftwareSerial.h"
#include "MHZ19.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <time.h>
#include "customChar.h"
#define BACKLIGHT_PWM_CHANNEL 0
#define BACKLIGHT_PWM_RES 8
#define BACKLIGHT_PWM_FREQUENCY 5000
#define BACKLIGHT_LED_PIN 25

DHT sens(27, DHT11);

int temp;
int humi;
bool sensio; // sensor in ordnung i.o.
int co2;
int updateTimer;
struct tm timeinfo;
char firstLineLcd[16];
char secondLineLcd[16];

#define RXD2 16
#define TXD2 17
#define BAUDRATE 9600
#define GMT_OFFSET_SECONDS 3600
#define DAYLIGHT_OFFSET_SECONDS 3600
const char *NTPSERVER = "pool.ntp.org";

char LOCAL_SSID[] = "NETGEAR-Guest";
char LOCAL_PASS[] = "alder,36";

MHZ19 myMHZ19;

LiquidCrystal_I2C lcd(0x27, 16, 2);

/* LCD Format:
=================
16:56--C:2000--
T:22*--H:20%---
=================
*/
void lcdFormat()
{
    sprintf(firstLineLcd, "%02d:%02d  C:%d", timeinfo.tm_hour, timeinfo.tm_min, co2);
    sprintf(secondLineLcd, "T:%d   H:%d%%", temp, humi);
}
void lcdWrite()
{
    lcdFormat();
    lcd.setCursor(0, 0);
    lcd.print(firstLineLcd);
    lcd.setCursor(0, 1);
    lcd.print(secondLineLcd);
    lcd.setCursor(4, 1);
    lcd.write(0);

    lcd.setCursor(14, 0);
    if (co2 < 1000)
    {
        lcd.write(3);
        return;
    }
    else if (co2 >= 1000 && co2 < 2000)
    {
        lcd.write(2);
    }
    else
    {
        lcd.write(1);
    }

    lcd.setCursor(14, 1);
    if (40 <= humi && humi <= 60)
    {
        lcd.write(1);
    }
    else if (humi < 40 || humi > 60)
    {
        lcd.write(3);
    }
}

void sensRead()
{
    temp = sens.readTemperature(); // Temp auslesen
    humi = sens.readHumidity();    // humidity auslesen
    co2 = myMHZ19.getBackgroundCO2();
    temp = temp - 1;
    //==========================================================

    // error handling
    if (isnan(humi) || isnan(temp))
    {
        sensio = false;
        return;
    }
    else
    {
        sensio = true;
        return;
    }
}

void readNptTime()
{

    if (!getLocalTime(&timeinfo))
    {
        return;
    }
}

void setup()
{

    // initialize sesors and start serial
    sens.begin();
    Serial2.begin(BAUDRATE, SERIAL_8N1, RXD2, TXD2);
    myMHZ19.begin(Serial2);

    // connect to wifi
    ConnectWlan(LOCAL_PASS, LOCAL_SSID);
    printWifiStatus();

    // initialize ld and custom char(°)
    lcd.init();
    lcd.backlight();
    lcd.createChar(0, degree);
    lcd.createChar(1, arrowUp);
    lcd.createChar(2, line);
    lcd.createChar(3, arrowDown);

    // configure and initialize NPT time
    configTime(GMT_OFFSET_SECONDS, DAYLIGHT_OFFSET_SECONDS, NTPSERVER);
}

void loop()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        ConnectWlan(LOCAL_PASS, LOCAL_SSID);
    }

    if (updateTimer < millis())
    {
        updateTimer = millis() + 1000;
        sensRead();
        lcdWrite();
        readNptTime();
    }
}
