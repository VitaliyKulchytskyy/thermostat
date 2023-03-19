#pragma once
#include "Arduino.h"
#include "Metadata.h"


/*Metadata::Metadata(const DateFormat &mDate, const TemperatureFormat &mTemp, const LogFormat &mLog)
        : m_date(mDate),
          m_temp(mTemp),
          m_log(mLog)
{}*/

uint8_t* Metadata::serialize() const {
    /*
    auto* output = new uint8_t[formatSize()]{};

    auto* date_raw = m_date.serialize();
    memcpy(output, date_raw, m_date.formatSize());
    delete date_raw;

    auto* temp_raw = m_temp.serialize();
    memcpy(&output[m_date.formatSize() - 1], temp_raw, m_temp.formatSize());
    delete temp_raw;

    auto log_raw = m_log.serialize();
    memcpy(&output[m_date.formatSize() + m_log.formatSize() - 1], log_raw, m_temp.formatSize());
    delete log_raw;

    return output;*/
}

void Metadata::toSerial() const {
    /*
    Serial.println("Date:");
    m_date.toSerial();
    Serial.println("Temperatures:");
    m_temp.toSerial();
    Serial.println("Log:");
    m_log.toSerial();
    Serial.println("==============");
     */
    Serial.println("metadata");
}

size_t Metadata::formatSize() const {
    return 1;//m_date.formatSize() + m_temp.formatSize() + m_log.formatSize();
}
