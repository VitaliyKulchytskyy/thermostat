#include "metadata_formats/LightSensorFormat.h"

void light_t::begin() {
    Wire.begin();
    m_lightMeter.begin(BH1750::CONTINUOUS_LOW_RES_MODE);
}

size_t light_t::size() const {
    return sizeof(m_lux);
}

uint8_t *light_t::serialize() const {
    auto* output = new uint8_t[this->size()];
    memcpy(output, &m_lux, sizeof(m_lux));
    return output;
}

log_t light_t::request() {
    while (!m_lightMeter.measurementReady(true)) {
        yield();
    }

    m_lux = m_lightMeter.readLightLevel();
    m_lightMeter.configure(BH1750::ONE_TIME_HIGH_RES_MODE);

    return 0;
}

#if (defined DEBUG_REQUEST_MODE || defined DEBUG_REQUEST_MODE_LOW_MEMORY)
void light_t::toSerial() const {
    Serial.print("Light sensor: ");
    Serial.println(m_lux);
}
#endif

BH1750 light_t::m_lightMeter = 0x23;  // or 0x5C
