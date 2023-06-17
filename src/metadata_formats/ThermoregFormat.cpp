#include "metadata_formats/ThermoregFormat.h"


void thermoreg_t::begin() {
    pinMode(PIN_RELAY_PUMP, OUTPUT);
    getRelay(m_temp.insideTemperatureC);
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
    static float prevInput = 0.0;
    static bool relayStat = false;
    static const float hysteresis = m_hysteresis / 2;
    float signal;

    if (m_inertia > 0) {
        signal = inputTempC + ((inputTempC - prevInput) / THREAD_THERMOSTAT_MS) * m_inertia;
        prevInput = inputTempC;
    } else {
        signal = inputTempC;
    }

    if (signal < (m_pointC - hysteresis))       relayStat = false;
    else if (signal > (m_pointC + hysteresis))  relayStat = true;

    #ifdef PLOT_MODE
        Serial.print("inputTempC:");
        Serial.print(inputTempC);
        Serial.print(",");

        Serial.print("setPointC:");
        Serial.print(SET_POINT_C);
        Serial.print(",");

        Serial.print("lowHysteresis:");
        Serial.print(SET_POINT_C - hysteresis);
        Serial.print(",");

        Serial.print("highHysteresis:");
        Serial.print(SET_POINT_C + hysteresis);
        Serial.print(",");

        Serial.print("relayState_x10:");
        Serial.println(relayStat * 10);
    #endif

    return relayStat;
}