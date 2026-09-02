#include <Arduino.h>

#include "config/config.h"
#include "scanner/scanner.h"
#include "display/display.h"


void setup()
{
    Serial.begin(SERIAL_BAUDRATE);

    displayInit();

    scannerInit();

    scannerStartAll();
}


void loop()
{
    scannerProcess();
}