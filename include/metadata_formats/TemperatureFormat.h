#pragma once
#include "Settings.h"


/**
 * The structure of temperature parameters used in the .bin file format
 */
struct temperature_t {
public:
    uint8_t inside;
    uint8_t outside;
public:
    static temperature_t readModuleInstance() {
        return temperature_t{35, 10};
    }

    void toSerial() const {
        Serial.print("inside: ");
        Serial.print(inside);
        Serial.print("C | outside: ");
        Serial.print(outside);
        Serial.println("C");
    }
};
