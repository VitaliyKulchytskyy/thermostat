#pragma once
#include "Arduino.h"
#include "FormatBase.h"

/// On thermoregulations mode
#define THERMOREG_ON    0x80
/// Off thermoregulations mode
#define THERMOREG_OFF   0x00
/// Does SD card enable
#define SD_CARD_ENABLE  0x00
/// Does SD card unable
#define SD_CARD_UNABLE  0x01


class LogFormat final: public FormatBase {
private:
    uint8_t m_log = 0;
public:
    LogFormat() = default;
    LogFormat(uint8_t logCode) : m_log(logCode) {}
public:
    uint8_t *serialize() const override {
        return new uint8_t[1]{m_log};
    }

    void toSerial() const override {
        Serial.print("Log: ");
        Serial.println(m_log);
    }

    size_t formatSize() const override {
        return 1;
    }

    uint8_t getStatus() const {
        return m_log;
    }

    void setStatus(uint8_t status) {
        m_log = status;
    }
};

