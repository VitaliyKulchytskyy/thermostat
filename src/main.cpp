#include <Arduino.h>
#include "Settings.h"
#include "ThreadHandler.h"


void setup() {
    #if isDebugModeEnable()
        Serial.begin(SERIAL_BEGIN_BAUD);
    #endif

    ThreadHandler::begin();
}

void loop() {
    ThreadHandler::run();
}