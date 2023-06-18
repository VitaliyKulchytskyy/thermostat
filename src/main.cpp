#include <Arduino.h>
#include "Settings.h"
#include "ThreadHandler.h"

// TODO: знов мало пам'яті для запису на внутрішню пам'ять


void setup() {
    #if isDebugModeEnable()
        Serial.begin(SERIAL_BEGIN_BAUD);
    #endif

    ThreadHandler::begin();
}

void loop() {
    ThreadHandler::run();
}