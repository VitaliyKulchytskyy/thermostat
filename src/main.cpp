#include <Arduino.h>
#include "ThreadHandler.h"

/*
 * TODO:
 *  [+] Fix stack push & pop
 *  [-] Serialize metadata on the SD card
 *  [-] Relay algorithm for thermoregulation
 */

void setup() {
    Serial.begin(9600);
    if(!SD.begin(SD_CHIP_SELECT))
        Serial.println("SD card is unable");
    ThreadHandler::begin();
}

void loop() {
    delay(2000);
    ThreadHandler::testSaveSd();
}



