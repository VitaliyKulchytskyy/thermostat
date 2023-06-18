#include "metadata_formats/ThermoregFormat.h"


void thermoreg_t::begin() {
    pinMode(PIN_RELAY_PUMP, OUTPUT);

    #ifdef DEBUG_SAVED_CONFIG
        Serial.print("Point C: ");
        Serial.println(m_pointC);
        Serial.print("Hysteresis: ");
        Serial.println(m_hysteresis);
        Serial.print("Inertia: ");
        Serial.println(m_inertia);
    #endif
}

size_t thermoreg_t::size() const {
    return 0;
}

uint8_t *thermoreg_t::serialize() const {
    return nullptr;
}

log_t thermoreg_t::request() {
    const float tempC = m_temp.insideTemperatureC;
    return thermoregulation(tempC);
}

log_t thermoreg_t::thermoregulation(float tempC) const {
    const bool state = getRelay(tempC);
    digitalWrite(PIN_RELAY_PUMP, state);

    static bool prevState = false;

    if (prevState && state)
        return setState(1 << INFO_THERMOREGULATION_PROCESS, prevState, state);
    else if(!prevState && !state)
        return setState(0, prevState, state);
    else if(!prevState &&  state)
        return setState(1 << INFO_THERMOREGULATION_START, prevState, state);
    else if( prevState && !state)
        return setState(1 << INFO_THERMOREGULATION_END, prevState, state);

    return 0;
}

log_t thermoreg_t::setState(log_t logCode, bool &outState, bool getState) {
    outState = getState;
    return logCode;
}

bool thermoreg_t::getRelay(float inputTempC) const {
    static float prevInput = inputTempC;
    static bool relayState = false;

    static const float halfHysteresis = m_hysteresis / 2;
    float signal;

    if (m_inertia > 0) {
        signal = inputTempC + ((inputTempC - prevInput) / ((float)m_intervalMs / 1000)) * m_inertia;
        prevInput = inputTempC;
    } else {
        signal = inputTempC;
    }

    if (signal < (m_pointC - halfHysteresis))       relayState = false;
    else if (signal > (m_pointC + halfHysteresis))  relayState = true;

    #ifdef PLOT_MODE
        Serial.print("signal:");
        Serial.print(signal);
        Serial.print(",");

        Serial.print("inputTempC:");
        Serial.print(inputTempC);
        Serial.print(",");

        Serial.print("setPointC:");
        Serial.print(m_pointC);
        Serial.print(",");

        Serial.print("lowHysteresis:");
        Serial.print(m_pointC - halfHysteresis);
        Serial.print(",");

        Serial.print("highHysteresis:");
        Serial.print(m_pointC + halfHysteresis);
        Serial.print(",");

        Serial.print("relayState_x10:");
        Serial.println(relayState * 10);
    #endif

    return relayState;
}