#include "metadata_formats/TemperatureFormat.h"

/* Compile-time calculation */
constexpr double revPivotTemp = 1.0 / (PIVOT_TEMPERATURE + 273.15);
constexpr double revBetaCoef = 1.0 / BETA_COEFFICIENT_NTC;

OneWire temperature_t::m_oneWire = PIN_ONE_WIRE_BUS;
#if isDSModule(TEMP_TYPE_INSIDE) || isDSModule(TEMP_TYPE_OUTSIDE)
    DallasTemperature temperature_t::m_sensors = &m_oneWire;
    DeviceAddress temperature_t::m_dsAddr{0x28, 0x91, 0xFF, 0xF9, 0xD, 0x0, 0x0, 0xA3};
#endif

#if isDSModule(TEMP_TYPE_INSIDE) || isDSModule(TEMP_TYPE_OUTSIDE)
bool temperature_t::setTemperature(const DeviceAddress& deviceIndex, float &outTempC) {
    float tempC = m_sensors.getTempC(deviceIndex);

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
        m_sensors.begin();
        m_sensors.setResolution(m_dsAddr, TEMPERATURE_DS_PRECISION, true);
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
    m_sensors.requestTemperatures();
    bool setInsideTemp, setOutsideTemp;

    #if isDSModule(TEMP_TYPE_INSIDE)
        setInsideTemp = setTemperature(m_dsAddr, insideTemperatureC);
    #else
        setInsideTemp = setTemperature(PIN_TEMP_MODULE_NTC, insideTemperatureC);
    #endif

    #if isDSModule(TEMP_TYPE_OUTSIDE)
        setOutsideTemp = setTemperature(m_dsAddr, outsideTemperatureC);
    #else
        setOutsideTemp = setTemperature(PIN_TEMP_MODULE_NTC, outsideTemperatureC);
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
    return m_sensors.isParasitePowerMode();
}
#endif