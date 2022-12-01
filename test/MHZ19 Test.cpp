#include "MHZ19.h"
#include <Arduino.h>
#include <SoftwareSerial.h> //  Remove if using HardwareSerial or non-uno compatabile device
#include <Wire.h>
#define RXD2 16
#define TXD2 17
#define BAUDRATE 9600 // Native to the sensor (do not change)

MHZ19 myMHZ19;

void setup()
{
    Serial.begin(9600);
    // Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);
    Serial2.begin(BAUDRATE, SERIAL_8N1, RXD2, TXD2);
    myMHZ19.begin(Serial2); // *Important, Pass your Stream reference here

    /*
      getVersion(char array[]) returns version number to the argument. The first 2 char are the major
      version, and second 2 bytes the minor version. e.g 02.11
    */

    char myVersion[4];
    myMHZ19.getVersion(myVersion);

    Serial.print("\nFirmware Version: ");
    for (byte i = 0; i < 4; i++)
    {
        Serial.print(myVersion[i]);
        if (i == 1)
            Serial.print(".");
    }
    Serial.println("");

    Serial.print("Range: ");
    Serial.println(myMHZ19.getRange());
    Serial.print("Background CO2: ");
    Serial.println(myMHZ19.getBackgroundCO2());
    Serial.print("Temperature Cal: ");
    Serial.println(myMHZ19.getTempAdjustment());
    Serial.print("ABC Status: ");
    myMHZ19.getABC() ? Serial.println("ON") : Serial.println("OFF");
}

void loop()
{
    while (1 == 1)
        ;
}