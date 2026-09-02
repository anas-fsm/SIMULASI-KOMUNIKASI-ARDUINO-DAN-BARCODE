#include "display.h"

#include "../config/config.h"

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(
    LCD_ADDRESS,
    LCD_COLUMNS,
    LCD_ROWS
);


// Menyimpan barcode terakhir
const char* lastBarcode[SCANNER_COUNT] =
{
    "---",
    "---",
    "---",
    "---"
};


// ==========================================
// INIT
// ==========================================

void displayInit()
{
    lcd.init();
    lcd.backlight();

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("SR-2000 SIMULATOR");

    lcd.setCursor(0, 1);
    lcd.print("Initializing...");

    delay(1000);
}


// ==========================================
// SEMUA SCANNER SCANNING
// ==========================================

void displayScanning()
{
    lcd.clear();

    for (int i = 0; i < SCANNER_COUNT; i++)
    {
        displaySetScanning(i);
    }
}


// ==========================================
// SATU SCANNER SCANNING
// ==========================================

void displaySetScanning(int scannerIndex)
{
    lcd.setCursor(0, scannerIndex);

    lcd.print("                    ");

    lcd.setCursor(0, scannerIndex);

    lcd.print("IP");
    lcd.print(scannerIndex + 1);
    lcd.print(": SCANNING...");
}


// ==========================================
// UPDATE BARCODE
// ==========================================

void displayUpdateScanner(
    int scannerIndex,
    const char* barcode
)
{
    lastBarcode[scannerIndex] = barcode;


    lcd.setCursor(0, scannerIndex);

    lcd.print("                    ");


    lcd.setCursor(0, scannerIndex);

    lcd.print("IP");
    lcd.print(scannerIndex + 1);
    lcd.print(": ");

    lcd.print(barcode);
}