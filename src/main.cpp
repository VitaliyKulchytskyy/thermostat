#include <Arduino.h>
#include "Settings.h"
#include "ThreadHandler.h"

/*
 * TODO: Add different type of debug
 *      TODO: Add plot builder [+]
 *           TODO: Disable other debug modes while plotting [+]
 *      TODO: Add thread debug [+]
 *      TODO: Implement DEBUG_SERIALIZATION [+]
 * TODO: Add NTC 10k module [+]
 *      TODO: Remove unused defines [+]
 * TODO: Test maximum size of stack
 * TODO: Final test
 * TODO: Draw the scheme of the device
 * TODO: Draw block-schemes of the algorithm
 */

void setup() {
    #if isDebugModeEnable()
        Serial.begin(SERIAL_BEGIN_BAUD);
    #endif

    ThreadHandler::begin();
}

void loop() {
    ThreadHandler::run();
}