#pragma once
#include "Settings.h"
#include "metadata_formats/DateFormat.h"

#ifdef DEBUG_CONFIG_DESERIALIZATION
    #include "SaveHandler.h"
#endif

/* Config */
#define DATE_FORMAT_OFFSET           0x00
#define DATE_FORMAT_SIZE             6
#define THREAD_FORMAT_OFFSET         0x06
#define THERMOSTAT_FORMAT_OFFSET     0x0E

#define THREAD_FORMAT_THERMOSTAT     (THREAD_FORMAT_OFFSET + 4)
#define THREAD_FORMAT_SAVE_DATA      (THREAD_FORMAT_THERMOSTAT + 4)
#define THERMOSTAT_FORMAT_SET_POINT  THERMOSTAT_FORMAT_OFFSET
#define THERMOSTAT_FORMAT_HYSTERESIS (THERMOSTAT_FORMAT_SET_POINT + 4)
#define THERMOSTAT_FORMAT_INERTIA    (THERMOSTAT_FORMAT_HYSTERESIS + 4)

/// The counts of config file parameters
#define CONFIG_LENGTH                11


class ConfigHandler {
public:
    explicit ConfigHandler(uint8_t *);

    ~ConfigHandler();

public:
    /**
     * Setup parameters of the thermoreg_t
     *
     * @param setTemperaturePoint after reaching this point the device starts thermoregulation
     * @param setHysteresis the value of hysteresis' window
     * @param setInertia the value of the thermoregulation system inertia
     */
    void setupThermostat(float& setTemperaturePoint, float& setHysteresis, float& setInertia) const;

    /**
     * Setup the clock module
     */
    void setupRTC() const;

    /**
     * Setup intervals of the threads
     *
     * @param setThermostatInterval sets the interval of thermostat thread invocation
     * @param setSaveDataInterval sets the interval of save data thread invocation
     */
    void setupThreads(uint32_t& setThermostatInterval, uint32_t& setSaveDataInterval) const;

private:
    /**
     * Converts first sizeof(N) bytes from a ptr to a value of N-type
     *
     * @return converted value of N-type
     */
    template<typename N>
    static N ptov(const uint8_t*);

private:
    uint8_t* m_pRawConfig;
};
