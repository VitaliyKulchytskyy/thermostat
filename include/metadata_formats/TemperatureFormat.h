#pragma once
#include "Settings.h"
#include "FormatBase.h"


/**
 * The structure of temperature parameters used in the .bin file format
 */
struct temperature_t: public FormatBase{
public:
    uint8_t inside;
    uint8_t outside;
public:
    explicit temperature_t(uint8_t in = 35, uint8_t out = 10)
        : inside(in), outside(out) {}

    static temperature_t readModuleInstance() {
        return temperature_t{35, 10};
    }  

    void toSerial() const final {
        Serial.print("inside: ");
        Serial.print(inside);
        Serial.print("C | outside: ");
        Serial.print(outside);
        Serial.println("C");
    }
public:
    size_t size() const final {
        return 2;
    }

    uint8_t *serialize() const final {
        return FormatBase::serialize();
    }
};
