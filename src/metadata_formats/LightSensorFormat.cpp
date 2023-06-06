#include "metadata_formats/LightSensorFormat.h"

void light_t::begin() {
    Wire.begin();
    m_lightMeter.begin(BH1750::CONTINUOUS_LOW_RES_MODE);
}

size_t light_t::size() const {
    return sizeof(float);
}

uint8_t *light_t::serialize() const {
    auto* output = new uint8_t[this->size()];
    memcpy(output, &m_lux, this->size());
    return output;
}

log_t light_t::request() {
    if (m_lightMeter.measurementReady())
        m_lux = m_lightMeter.readLightLevel();

    return 0;
}

#ifdef DEBUG_REQUEST_MODE
void light_t::toSerial() const {
    Serial.print("Light sensor: ");
    Serial.print(m_lux);
    //Serial.println(" lux");
}
#endif

BH1750 light_t::m_lightMeter = 0x23;
