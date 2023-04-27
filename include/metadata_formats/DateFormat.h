#pragma once
#include "iarduino_RTC.h"
#include "FormatBase.h"


/**
 * The structure of date parameters used in the .bin file format
 */
struct date_t: public FormatBase {
private:
    static iarduino_RTC _clock;
public:
    //date_t() = default;
public:
    static void begin() {
        _clock.begin();
    }

    void toSerial() const final {
        Serial.print(date_t::day());
        Serial.print(".");
        Serial.print(date_t::month());
        Serial.print(".20");
        Serial.print(date_t::year());
        Serial.print(" ");
        Serial.print(date_t::hours());
        Serial.print(":");
        Serial.print(date_t::minutes());
        Serial.print(":");
        Serial.println(date_t::seconds());
    }

    static void setTime() {
        // format: seconds (0-59), minutes (0-59), hours(0-23), day (1-31), month(1-12), year (0-99), name of day (1-7)
        _clock.settime(25, 50, 21, 14, 4, 23, 5);
    }
public:
    static uint8_t seconds() {
        return _clock.seconds;
    }

    static uint8_t minutes() {
        return _clock.minutes;
    }

    static uint8_t hours() {
        return _clock.hours;
    }

    static uint8_t day() {
        return _clock.day;
    }

    static uint8_t month() {
        return _clock.month;
    }

    static uint8_t year() {
        return _clock.year;
    }
public:
    size_t size() const override;

    uint8_t *serialize() const override;
};
