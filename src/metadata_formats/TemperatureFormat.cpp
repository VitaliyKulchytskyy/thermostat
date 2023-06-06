#include "metadata_formats/TemperatureFormat.h"

/* Compile-time calculation */
constexpr double revPivotTemp = 1.0 / (PIVOT_TEMPERATURE + 273.15);
constexpr double revBetaCoef = 1.0 / BETA_COEFFICIENT_NTC;


#if isDSModule(TEMP_TYPE_INSIDE) || isDSModule(TEMP_TYPE_OUTSIDE)
    OneWire temperature_t::oneWire = PIN_ONE_WIRE_BUS;
    DallasTemperature temperature_t::sensors = &oneWire;

    #if isDSModule(TEMP_TYPE_INSIDE)
        DeviceAddress temperature_t::inside  = {0x28, 0x3A, 0x2B, 0xDB, 0x58, 0x20, 0x1, 0xE0};
    #else
        DeviceAddress temperature_t::outside = {0x28, 0x91, 0xFF, 0xF9, 0xD,  0x0,  0x0, 0xA3};
    #endif
#endif

#if isDSModule(TEMP_TYPE_INSIDE) || isDSModule(TEMP_TYPE_OUTSIDE)
bool temperature_t::setTemperature(const DeviceAddress& deviceIndex, float &outTempC) {
    float tempC = sensors.getTempC(deviceIndex);

    if (tempC == DEVICE_DISCONNECTED_C)
        return true;

    outTempC = tempC;
    return false;
}
#endif

#if isNTCModule(TEMP_TYPE_INSIDE) || isNTCModule(TEMP_TYPE_OUTSIDE)
bool temperature_t::setTemperature(uint8_t readPin, float &outTempC) {
    double analog = analogRead(readPin);
    analog = analog / (1023 - analog);
    analog = (log(analog) / BETA_COEFFICIENT_NTC) + revPivotTemp;
    outTempC = (float)(1.0 / analog - 273.15);
    return false;
}
#endif

void temperature_t::begin() {
    #if isNTCModule(TEMP_TYPE_INSIDE) || isNTCModule(TEMP_TYPE_OUTSIDE)
        sensors.begin();

        #if isDSModule(TEMP_TYPE_INSIDE)
            sensors.setResolution(inside, TEMPERATURE_INSIDE_PRECISION, true);
        #endif

        #if isDSModule(TEMP_TYPE_OUTSIDE)
            sensors.setResolution(outside, TEMPERATURE_OUTSIDE_PRECISION, true);
        #endif
    #endif
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
    bool setInsideTemp, setOutsideTemp;

    #if isDSModule(TEMP_TYPE_INSIDE)
        setInsideTemp = setTemperature(inside, insideTemperatureC);
    #else
        setInsideTemp = setTemperature(PIN_TEMP_MODULE_INSIDE, insideTemperatureC);
    #endif

    #if isDSModule(TEMP_TYPE_OUTSIDE)
        setOutsideTemp = setTemperature(outside, outsideTemperatureC);
    #else
        setOutsideTemp = setTemperature(PIN_TEMP_MODULE_OUTSIDE, outsideTemperatureC);
    #endif

    log_t errorCode = (setInsideTemp << BAD_REQUEST_THERMOMETER_INSIDE)
                    | (setOutsideTemp << BAD_REQUEST_THERMOMETER_OUTSIDE);

    return errorCode;
}

#ifdef DEBUG_REQUEST_MODE
void temperature_t::toSerial() const {
    Serial.print("inside: ");
    Serial.print(insideTemperatureC);
    Serial.print("C | outside (+): ");
    Serial.print(outsideTemperatureC);
    Serial.println("C");
}
#endif

#if isDSModule(TEMP_TYPE_INSIDE) || isDSModule(TEMP_TYPE_OUTSIDE)
bool temperature_t::isParasitePower() const {
    return sensors.isParasitePowerMode();;
}
#endif