#pragma once
#include "Settings.h"
#include "FormatBase.h"
#include <OneWire.h>
#include <DallasTemperature.h>


/**
 * The structure of temperature parameters used in the .bin file format
 */
struct temperature_t: public FormatBase{
private:
    static OneWire oneWire;
    static DallasTemperature sensors;
    static DeviceAddress inside;
    static DeviceAddress outside;
private:
    float insideTemperatureC  = 0;
    float outsideTemperatureC = 0;
private:
    bool setTemperature(DeviceAddress& deviceAddr, float& outTempC) {
        float tempC = sensors.getTempC(deviceAddr);
        if (tempC == DEVICE_DISCONNECTED_C)
            return true;
        outTempC = tempC;
        return false;
    }
public:
    temperature_t() {
        sensors.begin();
    }

    uint8_t tick() {
        auto timer = millis();

        sensors.requestTemperatures();
        bool setInsideTemp = setTemperature(inside, insideTemperatureC);
        bool setOutsideTemp = setTemperature(outside, outsideTemperatureC);

        uint8_t errorCode = 0;
        errorCode |= (setInsideTemp << 0) | (setOutsideTemp << 1);

        delay(TICK_PERIOD_MS - (millis() - timer));
        return errorCode;
    }

    void toSerial() const override {
        Serial.print("inside (+): ");
        Serial.print(insideTemperatureC);
        Serial.print("C | outside: ");
        Serial.print(outsideTemperatureC);
        Serial.println("C");
    }

    bool isParasitePower() const {
        return sensors.isParasitePowerMode();
    }
public:
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
DeviceAddress temperature_t::inside  = {0x28, 0x91, 0xFF, 0xF9, 0xD,  0x0,  0x0, 0xA3};
DeviceAddress temperature_t::outside = {0x28, 0x3A, 0x2B, 0xDB, 0x58, 0x20, 0x1, 0xE0};