#include "scanner.h"

#include "../config/config.h"
#include "../display/display.h"

#include <Arduino.h>


// ==========================================
// SCANNER CONFIGURATION
// ==========================================

const char* scannerIP[SCANNER_COUNT] =
{
    "192.168.1.101",
    "192.168.1.102",
    "192.168.1.103",
    "192.168.1.104"
};


// Waktu tunggu setelah barcode muncul
// sebelum scanner mulai scanning lagi.
const unsigned long scannerDelay[SCANNER_COUNT] =
{
    10000,   // IP1 = 2 detik
    4000,   // IP2 = 4 detik
    6000,   // IP3 = 6 detik
    8000    // IP4 = 8 detik
};


// Lama tulisan "SCANNING..."
const unsigned long SCANNING_TIME = 3000;


// ==========================================
// BARCODE BUFFER
// ==========================================

// 12 digit barcode + '\0'
char generatedBarcode[SCANNER_COUNT][13];


// ==========================================
// SCANNER STATE
// ==========================================

enum ScannerState
{
    WAITING,
    SCANNING
};


ScannerState scannerState[SCANNER_COUNT];

unsigned long scannerStartTime[SCANNER_COUNT];


// ==========================================
// GENERATE RANDOM BARCODE
// ==========================================

void generateBarcode(char* buffer, int length)
{
    for (int i = 0; i < length; i++)
    {
        buffer[i] = '0' + random(0, 10);
    }

    buffer[length] = '\0';
}


// ==========================================
// INITIALIZE
// ==========================================

void scannerInit()
{
    // Seed random
    randomSeed(analogRead(A0));


    for (int i = 0; i < SCANNER_COUNT; i++)
    {
        scannerState[i] = WAITING;

        scannerStartTime[i] = 0;

        generatedBarcode[i][0] = '\0';
    }


    Serial.println();
    Serial.println("========================================");
    Serial.println("       SR-2000 MULTI SCANNER");
    Serial.println("       CONTINUOUS SIMULATION");
    Serial.println("========================================");
    Serial.println();

    Serial.println("STATUS > READY");
    Serial.println();
}


// ==========================================
// START ALL SCANNER
// ==========================================

void scannerStartAll()
{
    unsigned long now = millis();


    Serial.println("========================================");
    Serial.println("START READING");
    Serial.println("========================================");


    for (int i = 0; i < SCANNER_COUNT; i++)
    {
        // Awal sistem langsung masuk scanning
        scannerState[i] = SCANNING;

        scannerStartTime[i] = now;


        // Simulasi command SR-2000
        // LON[CR]

        Serial.print("TX [");
        Serial.print(scannerIP[i]);
        Serial.println("] > LON[CR]");


        // LCD
        displaySetScanning(i);
    }


    Serial.println();

    Serial.println("STATUS > ALL SCANNER SCANNING");
    Serial.println();
}


// ==========================================
// PROCESS SCANNER
// ==========================================

void scannerProcess()
{
    unsigned long now = millis();


    for (int i = 0; i < SCANNER_COUNT; i++)
    {
        unsigned long elapsed =
            now - scannerStartTime[i];


        // ==================================
        // STATE SCANNING
        // ==================================

        if (scannerState[i] == SCANNING)
        {
            // Sudah scanning selama 1 detik?
            if (elapsed >= SCANNING_TIME)
            {
                // ==================================
                // GENERATE BARCODE
                // ==================================

                generateBarcode(
                    generatedBarcode[i],
                    12
                );


                // ==================================
                // SIMULASI RESPONSE
                //
                // barcode[CR]
                // ==================================

                Serial.print("RX [");
                Serial.print(scannerIP[i]);
                Serial.print("] > ");

                Serial.print(generatedBarcode[i]);

                Serial.println("[CR]");


                // ==================================
                // STATUS
                // ==================================

                Serial.print("STATUS [");
                Serial.print(scannerIP[i]);
                Serial.println("] > READ OK");

                Serial.println();


                // ==================================
                // UPDATE LCD
                // ==================================

                displayUpdateScanner(
                    i,
                    generatedBarcode[i]
                );


                // ==================================
                // MASUK WAITING
                // ==================================

                scannerState[i] = WAITING;

                scannerStartTime[i] = now;
            }
        }


        // ==================================
        // STATE WAITING
        // ==================================

        else if (scannerState[i] == WAITING)
        {
            // Tunggu sesuai delay scanner
            if (elapsed >= scannerDelay[i])
            {
                // ==================================
                // MULAI SCANNING LAGI
                // ==================================

                scannerState[i] = SCANNING;

                scannerStartTime[i] = now;


                // ==================================
                // SIMULASI COMMAND LON
                // ==================================

                Serial.print("TX [");
                Serial.print(scannerIP[i]);
                Serial.println("] > LON[CR]");


                // ==================================
                // LCD SCANNING
                // ==================================

                displaySetScanning(i);
            }
        }
    }
}