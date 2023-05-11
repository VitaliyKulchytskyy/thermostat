#pragma once
#include "Settings.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define TEMPERATURE_PRECISION 9

/**
 * The structure of temperature parameters used in the .bin file format
 */
struct temperature_t: public FormatBase {
private:
    static OneWire oneWire;
    static DallasTemperature sensors;
    static DeviceAddress inside;
    static DeviceAddress outside;
public:
    float insideTemperatureC  = 0;
    float outsideTemperatureC = 0;
private:
    bool static setTemperature(const DeviceAddress& deviceIndex, float& outTempC) {
        float tempC = sensors.getTempC(deviceIndex);

        if (tempC == DEVICE_DISCONNECTED_C)
            return true;

        outTempC = tempC;
        return false;
    }
public:
    temperature_t() = default;
public:
    void begin() override {
        sensors.begin();
        sensors.setResolution(inside, TEMPERATURE_PRECISION, true);
        sensors.setResolution(outside, 12, true);
    }

    log_t request() override {
        sensors.requestTemperatures();
        bool setInsideTemp = setTemperature(inside, insideTemperatureC);
        bool setOutsideTemp = setTemperature(outside, outsideTemperatureC);

        uint8_t errorCode = 0;
        errorCode |= (setInsideTemp << BAD_REQUEST_THERMOMETER_INSIDE)
                  |  (setOutsideTemp << BAD_REQUEST_THERMOMETER_OUTSIDE);

        return errorCode;
    }

    void toSerial() const override {
        Serial.print("inside: ");
        Serial.print(insideTemperatureC);
        Serial.print("C | outside (+): ");
        Serial.print(outsideTemperatureC);
        Serial.println("C");
    }

    bool isParasitePower() const {
        return sensors.isParasitePowerMode();
    }

    size_t size() const override {
        return sizeof(insideTemperatureC) + sizeof(outsideTemperatureC);
    }

    uint8_t *serialize() const override {
        auto *output = new uint8_t[this->size()];
        memcpy(output, &insideTemperatureC, sizeof(insideTemperatureC));
        memcpy(&output[sizeof(insideTemperatureC)], &outsideTemperatureC, sizeof(outsideTemperatureC));
        return output;
    }
};

OneWire temperature_t::oneWire = ONE_WIRE_BUS;
DallasTemperature temperature_t::sensors = &oneWire;
DeviceAddress temperature_t::inside  = {0x28, 0x3A, 0x2B, 0xDB, 0x58, 0x20, 0x1, 0xE0};
DeviceAddress temperature_t::outside = {0x28, 0x91, 0xFF, 0xF9, 0xD,  0x0,  0x0, 0xA3};