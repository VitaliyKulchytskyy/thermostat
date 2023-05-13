#pragma once
#include "metadata_formats/LogFormat.h"
#include "metadata_formats/FormatBase.h"

/**
 * The structure handles the module to measure the illumination (in lux)
 */
struct light_t: public FormatBase {
private:
    uint16_t m_lux = 0;

public:
    light_t() = default;

public:
    void begin() override {}

    size_t size() const override;

    uint8_t *serialize() const override;

    log_t request() override;

#ifdef DEBUG_MODE
    void toSerial() const override;
#endif
};
