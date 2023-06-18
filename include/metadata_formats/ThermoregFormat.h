#pragma once
#include "metadata_formats/LogFormat.h"
#include "metadata_formats/FormatBase.h"
#include "metadata_formats/TemperatureFormat.h"

/**
 * The structure handles the thermoregulation process in the device
 */
struct thermoreg_t: public FormatBase {
private:
    /**
     * Handles the thermoregulation process by reading the input temperature.
     * The thermoregulation process proceeds by enabling or disabling the pump with cooling fluid
     *
     * @param tempC input temperature
     * @retval INFO_THERMOREGULATION_START thermoregulation process has been started
     * @retval INFO_THERMOREGULATION_PROCESS thermoregulation process is running
     * @retval INFO_THERMOREGULATION_END thermoregulation process has ended
     */
    log_t thermoregulation(float tempC) const;

    /**
     * Handles the returning of the thermoregulation method (macros)
     *
     * @param [in] logCode the log code of the thermoregulation process
     * @param [out] outState sets state before returning
     * @param [in] getState read and save state before returning
     * @return the log code
     */
    static log_t setState(log_t logCode, bool &outState, bool getState) ;

    /**
     * Enable or disable the pump based on inputTempC value and
     * measurement of temperature rise rate
     *
     * @param inputTempC input temperature
     * @retval true enable the pump
     * @retval false disable the pump
     */
    bool getRelay(float inputTempC) const;

public:
    thermoreg_t(float pointC,
                float hysteresis,
                float inertia,
                uint32_t intervalMs,
                temperature_t &mTemp)
        : m_pointC(pointC),
          m_hysteresis(hysteresis),
          m_inertia(inertia),
          m_intervalMs(intervalMs),
          m_rTemp(mTemp)
    {}

public:
    void begin() override;

    size_t size() const override;

    uint8_t *serialize() const override;

    log_t request() override;

#if (defined DEBUG_REQUEST_MODE || defined DEBUG_REQUEST_MODE_LOW_MEMORY)
    void toSerial() const override {}
#endif

private:
    /// The maximum acceptable temperature.
    /// After reaching this temperature the thermoregulation process will start
    const float m_pointC;
    /// The value of hysteresis. Used to reduce switching the relay
    const float m_hysteresis;
    /// The value of system inertia. Used to speed up or slow down thermoregulation process
    const float m_inertia;
    /// The interval of requesting temperature modules (in milliseconds)
    const uint32_t m_intervalMs;
    /// The reference on handler of temperature modules
    temperature_t& m_rTemp;
};