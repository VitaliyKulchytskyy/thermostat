#pragma once
#include "iarduino_RTC.h"


/**
 * The structure of date parameters used in the .bin file format
 */
struct date_t {
private:
    static iarduino_RTC _clock;
public:
    uint8_t day;
    uint8_t month;
    uint8_t year;
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
public:
    static date_t readModuleInstance() {
        _clock.begin();
        return date_t{_clock.day, _clock.month, _clock.year, _clock.Hours, _clock.minutes, _clock.seconds};
    }

    void toSerial() const {
        Serial.print(day);
        Serial.print(".");
        Serial.print(month);
        Serial.print(".20");
        Serial.print(year);
        Serial.print(" ");
        Serial.print(hours);
        Serial.print(":");
        Serial.print(minutes);
        Serial.print(":");
        Serial.println(seconds);
    }
};
