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
     * @param tempC input temperature
     * @retval INFO_THERMOREGULATION_START thermoregulation process has been started
     * @retval INFO_THERMOREGULATION_PROCESS thermoregulation process is running
     * @retval INFO_THERMOREGULATION_END thermoregulation process has ended
     */
    log_t thermoregulation(float tempC) const;

    /**
     * Handles the returning of the thermoregulation method (macros)
     * @param [in] logCode the log code of the thermoregulation process
     * @param [out] outState sets state before returning
     * @param [in] getState read and save state before returning
     * @return the log code
     */
    static log_t setState(log_t logCode, bool &outState, bool getState) ;

    /**
     * Enable or disable the pump based on inputTempC value and
     * measurement of temperature rise rate
     * @param inputTempC input temperature
     * @retval true enable the pump
     * @retval false disable the pump
     */
    bool getRelay(float inputTempC) const;

public:
    thermoreg_t(float mPointC, float mHysteresis, float mInertia, const temperature_t &mTemp)
        : m_pointC(mPointC),
          m_hysteresis(mHysteresis),
          m_inertia(mInertia),
          m_temp(mTemp)
    {}

public:
    void begin() override;

    size_t size() const override;

    uint8_t *serialize() const override;

    log_t request() override;

#ifdef DEBUG_REQUEST_MODE
    void toSerial() const override {}
#endif

private:
    const float m_pointC;
    const float m_hysteresis;
    const float m_inertia;
    const temperature_t& m_temp;
};