#pragma once
#include "iarduino_RTC.h"
#include "metadata_formats/LogFormat.h"
#include "metadata_formats/FormatBase.h"

/// Checks whether the RTC module is reset by comparing the pivot year
#define CHECKS_BY_THE_YEAR      23

/// The index of the hours parameter. Value: 0 - 23
#define READ_HOURS      0
/// The index of the minutes parameter. Value: 0 - 59
#define READ_MINUTES    1
/// The index of the seconds parameter. Value: 0 - 59
#define READ_SECONDS    2
/// The index of the day parameter. Value: 1 - 31
#define READ_DAY        3
/// The index of the month parameter. Value: 1 - 12
#define READ_MONTH      4
/// The index of the year parameter. Value: CHECKS_BY_THE_YEAR - 99
#define READ_YEAR       5
/// The number of the day. Value: 1 (Monday) - 7 (Sunday). Doesn't use.
#define NUM_OF_DAY      5  // Friday


/**
 * The structure handles RTC module and represents the current time on the device
 */
struct date_t: public FormatBase {
public:
    /// Sets the time on RTC from the device
    static void setTime();

    /// Sets the time on RTC from the deserialized array
    static void setTime(uint8_t*);

public:
    date_t() = default;

public:
    void begin() override;

    size_t size() const override;

    /// Time format: HH:MM:SS DD/MM/YY
    uint8_t *serialize() const override;

    log_t request() override;

#ifdef DEBUG_MODE
    void toSerial() const override;
#endif

public:
    /// Gets filename by current time on RTC module
    const char* getFilename() const;

private:
    /**
     * Checks whether time is reset on RTC module.
     * In a way to hard reset RTC take the battery off and turn down the device for a while.
     */
    bool isTimeReset() const;

private:
    static iarduino_RTC m_watch;

private:
    /// Value: 0 - 23
    uint8_t m_hours = 0;
    /// Value: 0 - 59
    uint8_t m_minutes = 0;
    /// Value: 0 - 59
    uint8_t m_seconds = 0;
    /// Value: 1 - 31
    uint8_t m_day = 1;
    /// Value: 1 - 12
    uint8_t m_month = 1;
    /// Value: 0 - 99
    uint8_t m_year = 0;
};