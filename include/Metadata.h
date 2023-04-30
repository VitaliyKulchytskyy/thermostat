#pragma once
#include "metadata_formats/LogFormat.h"
#include "metadata_formats/FormatBase.h"
#include "metadata_formats/DateFormat.h"
#include "metadata_formats/LightSensorFormat.h"
#include "metadata_formats/TemperatureFormat.h"

template<size_t N>
struct metadata_t: public FormatBase {
private:
    FormatBase* m_formats[N];
public:
    log_t requestLog = 0;
public:
    metadata_t(FormatBase* (&formats)[N]) {
        for(size_t i = 0; i < N; i++)
            m_formats[i] = formats[i];
    }

    metadata_t& operator= (const metadata_t& other) {
        if(this == &other)
            return *this;

        for(size_t i = 0; i < N; i++)
            this->m_formats[i] = other.m_formats[i];

        return *this;
    }
public:
    void begin() override {
        for(const auto& pFormat: m_formats)
            pFormat->begin();
    }

    size_t size() const override {
        size_t output = 0;
        for(const auto& format: m_formats)
            output += format->size();

        return output + sizeof(log_t);
    }

    uint8_t *serialize() const override {
        auto* output = new uint8_t[this->size()];
        size_t shift = 0;

        for(const auto& pFormat: m_formats) {
            auto pFormatRaw = pFormat->serialize();
            const uint8_t formatSize = pFormat->size();
            memcpy(&output[shift], pFormatRaw, formatSize);
            shift += formatSize;
            delete[] pFormatRaw;
        }
        memcpy(&output[shift], &requestLog, sizeof(log_t));

        return output;
    }

    void toSerial() const override {
        Log::printLogInfo(requestLog);
        for(const auto& format: m_formats)
            format->toSerial();
    }

    log_t request() override {
        requestLog = 0;

        for(const auto& format: m_formats)
            requestLog |= format->request();

        return requestLog;
    }
};
