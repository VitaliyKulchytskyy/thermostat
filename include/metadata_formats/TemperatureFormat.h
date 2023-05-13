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
    /**
     * Sets temperature by reading the DS18B20 modules
     * @param [in] deviceIndex index of the module
     * @param [out] outTempC set to the parameter
     * @retval true if the deviceIndex can be read
     * @retval false if the deviceIndex cannot be read
     */
    bool static setTemperature(const DeviceAddress& deviceIndex, float& outTempC);

public:
    temperature_t() = default;

public:
    void begin() override;

    size_t size() const override;

    uint8_t *serialize() const override;

    log_t request() override;

#ifdef DEBUG_MODE
    void toSerial() const override;
#endif

    /**
     * Reads whether the DS18B20 modules supplied by parasite power
     * @retval true the DS18B20 modules supplied by parasite power
     * @retval false the DS18B20 modules don't supply by parasite power
     */
    bool isParasitePower() const;

private:
    static OneWire oneWire;
    static DallasTemperature sensors;
    /// The temperature module used to record temperature of the main device
    static DeviceAddress inside;
    /// The temperature module used to record outside temperature
    static DeviceAddress outside;

public:
    float insideTemperatureC  = 0;
    float outsideTemperatureC = 0;
};
