#include "Arduino.h"
#include "metadata_formats/DateFormat.h"

iarduino_RTC DateFormat::m_clock = RTC_DS1307;

DateFormat::DateFormat() {
    m_clock.begin();
    Day = m_clock.day;
    Month = m_clock.month;
    Year = m_clock.year;
    Hours = m_clock.hours;
    Minutes = m_clock.minutes;
    Seconds = m_clock.seconds;
}

uint8_t* DateFormat::serialize() const {
    auto* output = new uint8_t[FORMAT_VOLUME]{};

    output[DAY_SHIFT] = Day;
    output[MONTH_SHIFT] = Month;
    output[YEAR_SHIFT] = Year;
    output[HOURS_SHIFT] = Hours;
    output[MINUTES_SHIFT] = Minutes;
    output[SECONDS_SHIFT] = Seconds;

    return output;
}

void DateFormat::toSerial() const {
    // DD.MM.20YY HH:MM:SS
    Serial.print(Day);
    Serial.print(".");
    Serial.print(Month);
    Serial.print(".20");
    Serial.print(Year);
    Serial.print(" ");
    Serial.print(Hours);
    Serial.print(":");
    Serial.print(Minutes);
    Serial.print(":");
    Serial.println(Seconds);
}

void DateFormat::updateClock(uint8_t day,   uint8_t month,   uint8_t year,
                             uint8_t hours, uint8_t minutes, uint8_t seconds) {
    m_clock.begin();
    m_clock.settime(seconds, minutes, hours, day, month, year, 1);
}

size_t DateFormat::formatSize() const {
    return FORMAT_VOLUME;
}
