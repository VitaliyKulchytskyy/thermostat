#pragma once

struct light_t: public FormatBase {
private:
    uint16_t m_lux = 0;
public:
    light_t() = default;
public:
    void begin() override {}

    size_t size() const override {
        return sizeof(uint16_t);
    }

    uint8_t *serialize() const override {
        auto* output = new uint8_t[this->size()];
        memcpy(output, &m_lux, this->size());
        return output;
    }

    void toSerial() const override {
        Serial.print("Light sensor: ");
        Serial.print(m_lux);
        Serial.println(" lux");
    }

    uint8_t request() override {
        m_lux = analogRead(A0);
        return 0;
    }
};
