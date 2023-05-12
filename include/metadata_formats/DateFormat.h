#pragma once
#include "iarduino_RTC.h"

#define READ_HOURS      0
#define READ_MINUTES    1
#define READ_SECONDS    2
#define READ_DAY        3
#define READ_MONTH      4
#define READ_YEAR       5
#define NUM_OF_DAY      5 // Friday

/**
 * The structure of date parameters used in the .bin file format
 */
struct date_t: public FormatBase {
private:
    static iarduino_RTC m_watch;
private:
    uint8_t m_hours = 0;
    uint8_t m_minutes = 0;
    uint8_t m_seconds = 0;
    uint8_t m_day = 1;
    uint8_t m_month = 1;
    uint8_t m_year = 0;
public:
    date_t() = default;
public:
    void begin() override {
        m_watch.begin();
    }

    log_t request() override {
        m_watch.gettime();
        m_hours = m_watch.Hours;
        m_minutes = m_watch.minutes;
        m_seconds = m_watch.seconds;
        m_day = m_watch.day;
        m_month = m_watch.month;
        m_year = m_watch.year;
        log_t errorCode = (this->isTimeReset() << ERROR_RTC_SET_UP);
        return errorCode;
    }

    void toSerial() const override {
        Serial.print(m_hours);
        Serial.print(":");
        Serial.print(m_minutes);
        Serial.print(":");
        Serial.print(m_seconds);
        Serial.print("\t");
        Serial.print(m_day);
        Serial.print(".");
        Serial.print(m_month);
        Serial.print(".20");
        Serial.println(m_year);
        Serial.print("Filename: ");
        Serial.println(getFilename());
    }

    static void setTime() {
        // format: seconds (0-59), minutes (0-59), hours(0-23), day (1-31), month(1-12), year (0-99), name of day (1-7)
        m_watch.settime(10, 7, 13, 12, 5, 23, NUM_OF_DAY);
    }

    static void setTime(uint8_t* const aDate) {
        m_watch.settime(aDate[READ_SECONDS],
                        aDate[READ_MINUTES],
                        aDate[READ_HOURS],
                        aDate[READ_DAY],
                        aDate[READ_MONTH],
                        aDate[READ_YEAR], NUM_OF_DAY);
    }

    const char* getFilename() const {
        static char filename[16]{};
        static uint8_t saveDay = m_day - 1;

        if(saveDay != m_day) {
            sprintf(filename, "%02i%02i20%02i.bin",
                    m_day,
                    m_month,
                    m_year);
            saveDay = m_day;
        }

        return filename;
    }
public:
    size_t size() const override {
        return sizeof(uint8_t) * 6;
    }

    /// HH:MM:SS DD/MM/YY
    uint8_t *serialize() const override {
        auto* date = new uint8_t[this->size()] {m_hours,
                                                m_minutes,
                                                m_seconds,
                                                m_day,
                                                m_month,
                                                m_year};

        return date;
    }
private:
    bool isTimeReset() const {
        return m_year < 23;
    }
};

iarduino_RTC date_t::m_watch = RTC_DS3231;