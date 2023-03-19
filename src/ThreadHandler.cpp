#include "ThreadHandler.h"

void ThreadHandler::begin() {
    m_thermoregulation.setInterval(THREAD_THERMOSTAT_INTERVAL_MS);
    m_thermoregulation.onRun(Callback::thermostat);
    m_threads.add(&m_thermoregulation);

    m_dataCollector.setInterval(THREAD_DATA_COLLECTOR_INTERVAL_MS);
    m_dataCollector.onRun(Callback::dataCollector);
    m_threads.add(&m_dataCollector);
}

void ThreadHandler::run() {
    m_threads.run();
}

void Callback::dataCollector() {
    const DateFormat d = DateFormat();
    Metadata metadata{}; // = Metadata(d, temperature, m_log);
    saveHnd.add(metadata);
    saveHnd.unload();
}

void Callback::thermostat() {
    temperature = TemperatureFormat();
    Serial.println("-> read temperature modules");
}
