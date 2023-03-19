#include "ThreadHandler.h"

void ThreadHandler::begin() {
    //DateFormat::updateClock(19, 3, 23, 23, 12, 20);
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
    DateFormat d = DateFormat().readTime();
    Metadata metadata = Metadata(d, temperature, m_log);
    metadata.toSerial();
    Serial.println("==========");
    saveHnd.add(metadata);
    saveHnd.unload();
}

void Callback::thermostat() {
    temperature = TemperatureFormat().readTemperature();
    Serial.println("-> read temperature modules");
}
