#include <Arduino.h>
#include "ThreadHandler.h"

/*
 * TODO:
 *  [-] Fix stack push & pop
 *  [-] Serialize metadata on the SD card
 *  [-] Relay algorithm for thermoregulation
 */


void setup() {
    Serial.begin(9600);
    ThreadHandler::begin();
}

void loop() {
    ThreadHandler::run();
}