#include "metadata_formats/TemperatureFormat.h"


OneWire temperature_t::oneWire = ONE_WIRE_BUS;
DallasTemperature temperature_t::sensors = &oneWire;
DeviceAddress temperature_t::inside  = {0x28, 0x3A, 0x2B, 0xDB, 0x58, 0x20, 0x1, 0xE0};
DeviceAddress temperature_t::outside = {0x28, 0x91, 0xFF, 0xF9, 0xD,  0x0,  0x0, 0xA3};

bool temperature_t::setTemperature(const DeviceAddress& deviceIndex, float &outTempC) {
    float tempC = sensors.getTempC(deviceIndex);

    if (tempC == DEVICE_DISCONNECTED_C)
        return true;

    outTempC = tempC;
    return false;
}

void temperature_t::begin() {
    sensors.begin();
    sensors.setResolution(inside, TEMPERATURE_PRECISION, true);
    sensors.setResolution(outside, 12, true);
}

size_t temperature_t::size() const {
    return sizeof(insideTemperatureC) + sizeof(outsideTemperatureC);
}

uint8_t *temperature_t::serialize() const {
    auto output = new uint8_t[this->size()];
    memcpy(output, &insideTemperatureC, sizeof(insideTemperatureC));
    memcpy(&output[sizeof(insideTemperatureC)], &outsideTemperatureC, sizeof(outsideTemperatureC));
    return output;
}

log_t temperature_t::request() {
    sensors.requestTemperatures();
    bool setInsideTemp = setTemperature(inside, insideTemperatureC);
    bool setOutsideTemp = setTemperature(outside, outsideTemperatureC);

    log_t errorCode = (setInsideTemp << BAD_REQUEST_THERMOMETER_INSIDE)
                    | (setOutsideTemp << BAD_REQUEST_THERMOMETER_OUTSIDE);

    return errorCode;
}

#ifdef DEBUG_MODE
void temperature_t::toSerial() const {
    Serial.print("inside: ");
    Serial.print(insideTemperatureC);
    Serial.print("C | outside (+): ");
    Serial.print(outsideTemperatureC);
    Serial.println("C");
}
#endif

bool temperature_t::isParasitePower() const {
    return sensors.isParasitePowerMode();;
}