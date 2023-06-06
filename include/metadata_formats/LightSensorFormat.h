#pragma once
#include <Wire.h>
#include "BH1750.h"
#include "metadata_formats/LogFormat.h"
#include "metadata_formats/FormatBase.h"

/**
 * The structure handles the module to measure the illumination (in lux)
 */
struct light_t: public FormatBase {
private:
    float m_lux = 0;
    static BH1750 m_lightMeter;

public:
    light_t() = default;

public:
    void begin() override;

    size_t size() const override;

    uint8_t *serialize() const override;

    log_t request() override;

#ifdef DEBUG_REQUEST_MODE
    void toSerial() const override;
#endif
};
