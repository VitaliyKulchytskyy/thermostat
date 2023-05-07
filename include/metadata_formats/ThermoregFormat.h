#pragma once

#define THREAD_THERMOSTAT_MS    900
#define HYSTERESIS_C            4.0
#define SET_POINT_C             30
#define THERMOSTAT_INERTIA      0.5
#define RELAY_PIN               5

struct thermoreg_f: public FormatBase {
private:
    bool prevState = false;
    const temperature_t& m_temp;
public:
    explicit thermoreg_f(const temperature_t &mTemp) : m_temp(mTemp) {}
public:
    size_t size() const override {
        return 0;
    }

    uint8_t *serialize() const override {
        return nullptr;
    }

    void toSerial() const override {

    }

    log_t request() override {
        return thermoregulation(m_temp.outsideTemperatureC);
    }

    void afterRequest() override {

    }

    void begin() override {
        relayGet(m_temp.outsideTemperatureC);
    }
private:
    log_t thermoregulation(int8_t tempC) {
        const bool state = relayGet(tempC);
        log_t outputCode = 0;
        pinMode(RELAY_PIN, state);

        if (prevState && state) {
            outputCode = 1 << INFO_THERMOREGULATION_PROCESS;
            prevState = state;
            return outputCode;
        }
        else if(!prevState && !state){
            outputCode = 0;
            prevState = state;
            return outputCode;
        }
        else if(!prevState &&  state) outputCode = 1 << INFO_THERMOREGULATION_START;
        else if( prevState && !state) outputCode = 1 << INFO_THERMOREGULATION_END;

        prevState = state;
        return outputCode;
    }

    static bool relayGet(float input, float k = THERMOSTAT_INERTIA) {
        static float prevInput = 0.0;
        static bool relayStat = false;
        static const float hysteresis = HYSTERESIS_C / 2;
        float signal;

        if (k > 0) {
            signal = input + ((input - prevInput) / THREAD_THERMOSTAT_MS) * k;
            prevInput = input;
        } else {
            signal = input;
        }

        if (signal < (SET_POINT_C - hysteresis)) relayStat = false;
        else if (signal > (SET_POINT_C + hysteresis)) relayStat = true;

        return relayStat;
    }
};