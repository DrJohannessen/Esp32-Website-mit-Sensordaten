#include <Arduino.h>
#include <Wire.h>                   // Wire Bibliothek einbinden
#include <LiquidCrystal_I2C.h>      // Vorher hinzugefügte LiquidCrystal_I2C Bibliothek einbinden
LiquidCrystal_I2C lcd(0x27, 16, 2); // Hier wird festgelegt um was für einen Display es sich handelt. In diesem Fall eines mit 16 Zeichen in 2 Zeilen und der HEX-Adresse 0x27. Für ein vierzeiliges I2C-LCD verwendet man den Code "LiquidCrystal_I2C lcd(0x27, 20, 4)"

int humi = 50;
int temp = 23;

byte customChar[8] = {
    0b01100,
    0b10010,
    0b10010,
    0b01100,
    0b00000,
    0b00000,
    0b00000,
    0b00000};

void setup()
{
    Serial.begin(9600);
    lcd.init();                    // Im Setup wird der LCD gestartet
    lcd.backlight();               // Hintergrundbeleuchtung einschalten (lcd.noBacklight(); schaltet die Beleuchtung aus).
    lcd.createChar(0, customChar); // create a new custom character
}

void loop()
{
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(temp);
    lcd.write((byte)0);
    lcd.print("  H:");
    lcd.print(humi);
    lcd.print("%");
}