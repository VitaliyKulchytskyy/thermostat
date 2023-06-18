#pragma once
#include "metadata_formats/DateFormat.h"
#include "metadata_formats/LightSensorFormat.h"
#include "metadata_formats/ThermoregFormat.h"


/**
 * The structure of the format collection
 *
 * @tparam N the size of the input array of formats
 */
template<size_t N>
struct metadata_t: public FormatBase {
public:
    metadata_t(FormatBase* (&formats)[N]);

    metadata_t& operator= (const metadata_t& other);

public:
    void begin() override;

    size_t size() const override;

    uint8_t *serialize() const override;

    log_t request() override;

#if (defined DEBUG_REQUEST_MODE || defined DEBUG_REQUEST_MODE_LOW_MEMORY)
    void toSerial() const override;

    /// Prints the log code of the last request
    void printLog() const;
#endif

private:
    FormatBase* m_formats[N];

public:
    log_t requestLog = 0;
};

// --- Implementation of the template class ---
template<size_t N>
metadata_t<N>::metadata_t(FormatBase *(&formats)[N]) {
    for(size_t i = 0; i < N; i++)
        m_formats[i] = formats[i];
}

template<size_t N>
metadata_t<N> &metadata_t<N>::operator=(const metadata_t &other) {
    if(this == &other)
        return *this;

    for(size_t i = 0; i < N; i++)
        this->m_formats[i] = other.m_formats[i];

    return *this;
}

template<size_t N>
void metadata_t<N>::begin() {
    for(const auto& pFormat: m_formats)
        pFormat->begin();
}

template<size_t N>
size_t metadata_t<N>::size() const {
    size_t output = 0;
    for(const auto& format: m_formats)
        output += format->size();

    return output + sizeof(log_t);
}

template<size_t N>
uint8_t *metadata_t<N>::serialize() const {
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

template<size_t N>
log_t metadata_t<N>::request() {
    requestLog = 0;

    for(const auto& format: m_formats)
        requestLog |= format->request();

    return requestLog;
}

#if (defined DEBUG_REQUEST_MODE || defined DEBUG_REQUEST_MODE_LOW_MEMORY)
template<size_t N>
void metadata_t<N>::toSerial() const {
    this->printLog();
    for(const auto& format: m_formats)
        format->toSerial();
}

template<size_t N>
void metadata_t<N>::printLog() const {
    Log::printLogInfo(requestLog, HEX);
}
#endif