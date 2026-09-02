#ifndef DISPLAY_H
#define DISPLAY_H

void displayInit();

void displayScanning();

void displaySetScanning(int scannerIndex);

void displayUpdateScanner(
    int scannerIndex,
    const char* barcode
);

#endif