#pragma once
#include <OneWire.h>
#include <DallasTemperature.h>
#include "metadata_formats/LogFormat.h"
#include "metadata_formats/FormatBase.h"


/**
 * The structure handles temperature modules (DS18B20 & NTC_10K)
 */
struct temperature_t: public FormatBase {
private:
#if isDSModule(TEMP_TYPE_INSIDE) || isDSModule(TEMP_TYPE_OUTSIDE)
    /**
     * Sets temperature by reading the DS18B20 module
     *
     * @param [in] deviceIndex index of the module
     * @param [out] outTempC set to the parameter
     * @retval true if the deviceIndex can be read
     * @retval false if the deviceIndex cannot be read
     */
    bool static setTemperature(const DeviceAddress& deviceIndex, float& outTempC);
#endif

#if isNTCModule(TEMP_TYPE_INSIDE) || isNTCModule(TEMP_TYPE_OUTSIDE)
    /**
     * Sets temperature by reading the NTC_XK module
     *
     * @param [in] readPin pin of the temperature module
     * @param [out] outTempC set to the parameter
     * @retval true if the readPin can be read
     * @retval false if the readPin cannot be read
     */
    bool static setTemperature(uint8_t readPin, float& outTempC);
#endif

public:
    temperature_t() = default;

public:
    void begin() override;

    size_t size() const override;

    uint8_t *serialize() const override;

    log_t request() override;

#if (defined DEBUG_REQUEST_MODE || defined DEBUG_REQUEST_MODE_LOW_MEMORY)
    void toSerial() const override;
#endif

#if isDSModule(TEMP_TYPE_INSIDE) || isDSModule(TEMP_TYPE_OUTSIDE)
    /**
     * Reads whether the DS18B20 modules supplied by parasite power
     *
     * @retval true the DS18B20 modules supplied by parasite power
     * @retval false the DS18B20 modules don't supply by parasite power
     */
    bool isParasitePower() const;
#endif

private:
#if isDSModule(TEMP_TYPE_INSIDE) || isDSModule(TEMP_TYPE_OUTSIDE)
    static OneWire m_oneWire;
    static DallasTemperature m_sensors;
    /// The temperature module used to record temperature of the main device
    static DeviceAddress m_dsAddr;
#endif

public:
    float insideTemperatureC  = 0;
    float outsideTemperatureC = 0;
};
