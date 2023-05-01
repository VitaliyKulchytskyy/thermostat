#include <Arduino.h>
//#include <TimerOne.h>
#include <Thread.h>
#include <ThreadController.h>
#include "Metadata.h"
#include "SaveHandler.h"

/*
 * TODO: Implement Metadata.h [+]
 *      TODO: Implement serialization on SD [+]
 * TODO: Implement logs [+]
 *      TODO: Implement all error codes
 * TODO: Implement watch backup
 * TODO: Implement pseudo-threads [+]
 * TODO: Implement thermoregulation
 */

date_t g_date;
temperature_t g_tempC;
light_t g_light;
FormatBase* g_formats[3]{&g_date, &g_tempC, &g_light};

constexpr size_t formatsNum = sizeof(g_formats) / sizeof(g_formats[0]);
metadata_t<formatsNum> metadata(g_formats);
const uint8_t mdtSize = metadata.size();
SaveHandler saver(mdtSize);

ThreadController m_threads = ThreadController();
Thread m_thermoregulation = Thread();
Thread m_dataCollector = Thread();

void printRawData(const FormatBase& format, uint8_t outputSys = HEX) {
    uint8_t *rawFormat = format.serialize();
    const uint8_t formatSize = format.size();

    Serial.print("Raw format: ");
    for(uint8_t i = 0; i < formatSize; i++) {
        Serial.print(rawFormat[i], outputSys);
        Serial.print(" ");
    }

    Serial.println();
    delete[] rawFormat;
}

void saveMetadataOnSD() {
    static bool isStackOverflow = false;
    static bool isSaved = true;

    metadata.request();
    metadata.requestLog |= ((isStackOverflow && !isSaved) << ERROR_FILE_STACK_OVERFLOW);
    //metadata.toSerial();

    auto temp = metadata.serialize();
    isStackOverflow = saver.add(temp);
    isSaved = saver.unload(g_date.getFilename());
    delete[] temp;
}

void readTemperature() {
    g_tempC.request();
    g_tempC.toSerial();
}

void setup() {
    Serial.begin(9600);
    metadata.begin();

    m_dataCollector.setInterval(2000);
    m_dataCollector.onRun(saveMetadataOnSD);
    m_threads.add(&m_dataCollector);

    m_thermoregulation.setInterval(800);
    m_thermoregulation.onRun(readTemperature);
    m_threads.add(&m_thermoregulation);
}

void loop() {
    m_threads.run();
}