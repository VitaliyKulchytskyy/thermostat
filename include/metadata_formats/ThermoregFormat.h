#pragma once
#include "metadata_formats/LogFormat.h"
#include "metadata_formats/FormatBase.h"
#include "metadata_formats/TemperatureFormat.h"

/**
 * The structure handles the thermoregulation process in the device
 */
struct thermoreg_f: public FormatBase {
private:
    /**
     * Handles the thermoregulation process by reading the input temperature.
     * The thermoregulation process proceeds by enabling or disabling the pump with cooling fluid
     * @param tempC input temperature
     * @retval INFO_THERMOREGULATION_START thermoregulation process has been started
     * @retval INFO_THERMOREGULATION_PROCESS thermoregulation process is running
     * @retval INFO_THERMOREGULATION_END thermoregulation process has ended
     */
    static log_t thermoregulation(float tempC);

    /**
     * Handles the returning of the thermoregulation method
     * @param [in] logCode the log code of the thermoregulation process
     * @param [out] outState sets state before returning
     * @param [in] getState read and save state before returning
     * @return the log code
     */
    static log_t setState(log_t logCode, bool &outState, bool getState);

    /**
     * Enable or disable the pump based on inputTempC value and
     * measurement of temperature rise rate
     * @param inputTempC input temperature
     * @param k coefficient of system inertia
     * @retval true enable the pump
     * @retval false disable the pump
     */
    static bool relayGet(float inputTempC, float k = THERMOSTAT_INERTIA);

public:
    explicit thermoreg_f(const temperature_t &mTemp) : m_temp(mTemp) {}

public:
    void begin() override;

    size_t size() const override;

    uint8_t *serialize() const override;

    log_t request() override;

#ifdef DEBUG_MODE
    void toSerial() const override {}
#endif

private:
    const temperature_t& m_temp;
};