#include "Arduino.h"
#include "metadata_formats/DateFormat.h"

iarduino_RTC date_t::m_watch = RTC_DS3231;

void date_t::begin() {
    m_watch.begin();
}

log_t date_t::request() {
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

#ifdef DEBUG_REQUEST_MODE
void date_t::toSerial() const {
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
#endif

const char *date_t::getFilename() const {
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

size_t date_t::size() const {
    return sizeof(uint8_t) * 6;
}

uint8_t *date_t::serialize() const {
    auto date = new uint8_t[this->size()] {m_hours,
                                            m_minutes,
                                            m_seconds,
                                            m_day,
                                            m_month,
                                            m_year};

    return date;
}

bool date_t::isTimeReset() const {
    return m_year < CHECKS_BY_THE_YEAR;
}

void date_t::setTime(uint8_t* aDate) {
    m_watch.settime(aDate[READ_SECONDS],
                    aDate[READ_MINUTES],
                    aDate[READ_HOURS],
                    aDate[READ_DAY],
                    aDate[READ_MONTH],
                    aDate[READ_YEAR], NUM_OF_DAY);
}

void date_t::setTime() {
    m_watch.settime(10, 7, 13, 12, 5, 23, NUM_OF_DAY);
}

