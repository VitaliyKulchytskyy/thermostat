#include "metadata_formats/LightSensorFormat.h"


size_t light_t::size() const {
    return sizeof(uint16_t);;
}

uint8_t *light_t::serialize() const {
    auto* output = new uint8_t[this->size()];
    memcpy(output, &m_lux, this->size());
    return output;
}

log_t light_t::request() {
    m_lux = analogRead(A0);
    return 0;
}

#ifdef DEBUG_MODE
void light_t::toSerial() const {
    Serial.print("Light sensor: ");
    Serial.print(m_lux);
    Serial.println(" lux");
}
#endif