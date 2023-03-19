#pragma once
#include "FormatBase.h"
#include "Settings.h"


#define INSIDE_TEMP_SHIFT       0
#define OUTSIDE_TEMP_SHIFT      1

using temp_t = uint8_t;
#define INSIDE_TEMP_VOLUME      sizeof(temp_t)
#define OUTSIDE_TEMP_VOLUME     sizeof(temp_t)

/// Volume of memory of TEMPERATURE format
#define FORMAT_VOLUME  (INSIDE_TEMP_VOLUME + OUTSIDE_TEMP_VOLUME)


/**
 * The structure of temperature parameters used in the .bin file format
 */
struct TemperatureFormat final: FormatBase {
public:
    /// Format: int8_t
    temp_t InsideTemp   = 0;
    /// Format: int8_t
    temp_t OutsideTemp  = 0;
public:
    TemperatureFormat() = default;

    TemperatureFormat(temp_t inTemperature, temp_t outTemperature)
            : InsideTemp(inTemperature),
              OutsideTemp(outTemperature) {}
public:
    uint8_t *serialize() const override;

    void toSerial() const override;

    size_t formatSize() const override;

    /**
     * Reads temperature from all modules
     * @return this ptr
     */
    TemperatureFormat& readTemperature();

    int calcDelta() const {
        return InsideTemp - OutsideTemp;
    }
};
