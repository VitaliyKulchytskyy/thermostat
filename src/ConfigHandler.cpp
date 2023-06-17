#include "ConfigHandler.h"


ConfigHandler::ConfigHandler(uint8_t *mpRawConfig)
    : m_pRawConfig(mpRawConfig)
{
    #ifdef DEBUG_CONFIG_DESERIALIZATION
        Serial.println("[Config debug]");
    #endif
}

ConfigHandler::~ConfigHandler() {
    delete[] m_pRawConfig;

    #ifdef DEBUG_CONFIG_DESERIALIZATION
        Serial.println("=================");
    #endif
}

void ConfigHandler::setupThermostat(float& setTemperaturePoint,
                                    float& setHysteresis,
                                    float& setInertia) const {
    setTemperaturePoint = ptov<float>(&m_pRawConfig[THERMOSTAT_FORMAT_SET_POINT]);
    setHysteresis       = ptov<float>(&m_pRawConfig[THERMOSTAT_FORMAT_HYSTERESIS]);
    setInertia          = ptov<float>(&m_pRawConfig[THERMOSTAT_FORMAT_INERTIA]);

    #ifdef DEBUG_CONFIG_DESERIALIZATION
        Serial.print("Temperature point: ");
        Serial.println(setTemperaturePoint);
        Serial.print("Hysteresis: ");
        Serial.println(setHysteresis);
        Serial.print("Inertia: ");
        Serial.println(setInertia);
    #endif
}

void ConfigHandler::setupRTC() const {
    auto temp = new uint8_t [DATE_FORMAT_SIZE]{};
    memcpy(temp, &m_pRawConfig[DATE_FORMAT_OFFSET], DATE_FORMAT_SIZE);
    //date_t::setTime(temp);

    #ifdef DEBUG_CONFIG_DESERIALIZATION
        DataInfo::printRawData(temp, 6, DEC);
    #endif

    delete[] temp;
}

void ConfigHandler::setupThreads(uint32_t& setThermostatInterval,
                                 uint32_t& setSaveDataInterval) const {
    setThermostatInterval = ptov<uint32_t>(&m_pRawConfig[THREAD_FORMAT_THERMOSTAT]);
    setSaveDataInterval   = ptov<uint32_t>(&m_pRawConfig[THREAD_FORMAT_SAVE_DATA]);

    #ifdef DEBUG_CONFIG_DESERIALIZATION
        Serial.print("Thermostat interval: ");
        Serial.println(setThermostatInterval);
        Serial.print("Save data interval: ");
        Serial.println(setSaveDataInterval);
    #endif
}

template<typename N>
N ConfigHandler::ptov(const uint8_t * pArray) {
    const uint8_t sizeOfType = sizeof(N);
    auto temp = new uint8_t[sizeOfType]{};
    memcpy(temp, pArray, sizeOfType);
    N output = (*(N*)temp);
    delete[] temp;

    return output;
}