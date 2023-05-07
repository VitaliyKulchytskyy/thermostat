#pragma once
#include "metadata_formats/LogFormat.h"
#include "metadata_formats/FormatBase.h"
#include "metadata_formats/DateFormat.h"
#include "metadata_formats/LightSensorFormat.h"
#include "metadata_formats/TemperatureFormat.h"
#include "metadata_formats/ThreadSyncFormat.h"
#include "metadata_formats/ThermoregFormat.h"

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
            const uint8_t formatSize = pFormat->size();
            if(formatSize == 0)
                continue;

            auto pFormatRaw = pFormat->serialize();
            memcpy(&output[shift], pFormatRaw, formatSize);
            shift += formatSize;
            delete[] pFormatRaw;
        }
        memcpy(&output[shift], &requestLog, sizeof(log_t));

        return output;
    }

    void toSerial() const override {
        this->printLog();
        for(const auto& format: m_formats)
            format->toSerial();
    }

    void printLog() const {
        Log::printLogInfo(requestLog, HEX);
    }

    log_t request() override {
        requestLog = 0;

        for(const auto& format: m_formats)
            requestLog |= format->request();

        return requestLog;
    }

    void afterRequest() override {}
};
