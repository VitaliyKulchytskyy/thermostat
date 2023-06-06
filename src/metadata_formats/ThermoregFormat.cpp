#include "metadata_formats/ThermoregFormat.h"


void thermoreg_f::begin() {
    relayGet(m_temp.insideTemperatureC);
}

size_t thermoreg_f::size() const {
    return 0;
}

uint8_t *thermoreg_f::serialize() const {
    return nullptr;
}

log_t thermoreg_f::request() {
    const float tempC = m_temp.insideTemperatureC;
    return thermoregulation(tempC);
}

log_t thermoreg_f::thermoregulation(float tempC) {
    const bool state = relayGet(tempC);
    pinMode(PIN_RELAY_PUMP, state);

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

log_t thermoreg_f::setState(log_t logCode, bool &outState, bool getState) {
    outState = getState;
    return logCode;
}

bool thermoreg_f::relayGet(float inputTempC, float k) {
    static float prevInput = 0.0;
    static bool relayStat = false;
    static const float hysteresis = HYSTERESIS_C / 2;
    float signal;

    if (k > 0) {
        signal = inputTempC + ((inputTempC - prevInput) / THREAD_THERMOSTAT_MS) * k;
        prevInput = inputTempC;
    } else {
        signal = inputTempC;
    }

    if (signal < (SET_POINT_C - hysteresis)) relayStat = false;
    else if (signal > (SET_POINT_C + hysteresis)) relayStat = true;

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