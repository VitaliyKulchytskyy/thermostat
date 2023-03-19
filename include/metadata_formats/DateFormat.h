#pragma once
#include "FormatBase.h"
#include "iarduino_RTC.h"

#define DAY_SHIFT       0
#define MONTH_SHIFT     (DAY_SHIFT + 1)
#define YEAR_SHIFT      (MONTH_SHIFT + 1)
#define HOURS_SHIFT     (YEAR_SHIFT + 1)
#define MINUTES_SHIFT   (HOURS_SHIFT + 1)
#define SECONDS_SHIFT   (MINUTES_SHIFT + 1)

#define DAY_VOLUME      1
#define MONTH_VOLUME    1
#define YEAR_VOLUME     1
#define HOURS_VOLUME    1
#define MINUTES_VOLUME  1
#define SECONDS_VOLUME  1

/// Volume of memory of DATE format
#define FORMAT_VOLUME  (DAY_VOLUME + \
                        MONTH_VOLUME + \
                        YEAR_VOLUME +\
                        HOURS_VOLUME + \
                        MINUTES_VOLUME + \
                        SECONDS_VOLUME)

/**
 * The structure of date parameters used in the .bin file format
 */
struct DateFormat final: FormatBase {
private:
    static iarduino_RTC m_clock;
public:
    /// Format: 1-31
    uint8_t Day     = 1;
    /// Format: 1-12
    uint8_t Month   = 1;
    /// Format: 0-99
    uint8_t Year    = 0;
    /// Format: 0-23
    uint8_t Hours   = 0;
    /// Format: 0-59
    uint8_t Minutes = 0;
    /// Format: 0-59
    uint8_t Seconds = 0;
public:
    DateFormat() = default;

    DateFormat(uint8_t day,   uint8_t month,   uint8_t year,
               uint8_t hours, uint8_t minutes, uint8_t seconds)
            : Day(day),
              Month(month),
              Year(year),
              Hours(hours),
              Minutes(minutes),
              Seconds(seconds) {
    }
public:
    uint8_t *serialize() const override;

    void toSerial() const override;

    size_t formatSize() const override;

    static void updateClock(uint8_t day,   uint8_t month,   uint8_t year,
                            uint8_t hours, uint8_t minutes, uint8_t seconds);
};
