#include <Arduino.h>
#include <Thread.h>
#include <ThreadController.h>
#include "Metadata.h"
#include "SaveHandler.h"

#define THREAD_THERMOSTAT_MS    1000
#define HYSTERESIS_C            4.0
#define SET_POINT_C             30
#define THERMOSTAT_INERTIA      0.5
#define RELAY_PIN               5

/*
 * TODO: Implement Metadata.h [+]
 *      TODO: Implement serialization on SD [+]
 * TODO: Implement logs [+]
 *      TODO: Implement all error codes
 * TODO: Implement watch backup
 * TODO: Implement pseudo-threads [+]
 * TODO: Implement thermoregulation [+]
 *      TODO: Код повернення скидається в 0 раніше запланованого [+]
 * TODO: Define DEBUG
 *      TODO: Cover over DEBUG flag all toSerial() methods (also in the interface definition)
 *      TODO: Cover over DEBUG flag all Serial outputs
 * TODO: Rewrite size() in Metadata.h
 */

date_t g_date;
temperature_t g_tempC;
light_t g_light;
thermoreg_f g_thermoreg(g_tempC);

FormatBase* g_formats[4]{&g_date, &g_tempC, &g_light, &g_thermoreg};
constexpr size_t formatsNum = sizeof(g_formats) / sizeof(g_formats[0]);
metadata_t<formatsNum> metadata(g_formats);

SaveHandler saver(metadata.size());

ThreadController m_threads = ThreadController();

/*void printRawData(const FormatBase& format, uint8_t outputSys = HEX) {
    uint8_t *rawFormat = format.serialize();
    const uint8_t formatSize = format.size();

    Serial.print("Raw format: ");
    for(uint8_t i = 0; i < formatSize; i++) {
        Serial.print(rawFormat[i], outputSys);
        Serial.print(" ");
    }

    Serial.println();
    delete[] rawFormat;
}*/


void saveMetadataImage() {
    static bool isStackOverflow = false;

    metadata.requestLog |= ((isStackOverflow) << ERROR_FILE_STACK_OVERFLOW);
    metadata.toSerial();
    Serial.println();

    auto temp = metadata.serialize();
    isStackOverflow = saver.add(temp);
    delete[] temp;
}

void saveMetadataOnSD() {
    if(!(1 & (metadata.requestLog >> INFO_THERMOREGULATION_START)
      || 1 & (metadata.requestLog >> INFO_THERMOREGULATION_END)))
        saveMetadataImage();

    saver.upload(g_date.getFilename());
}

void requestAllModules() {
    log_t returnCode = metadata.request();

    if(1 & (returnCode >> INFO_THERMOREGULATION_START)
    || 1 & (returnCode >> INFO_THERMOREGULATION_END))
        saveMetadataImage();
}

void setup() {
    Serial.begin(9600);
    metadata.begin();

    Thread m_thermoregulation = Thread();
    m_thermoregulation.setInterval(THREAD_THERMOSTAT_MS);
    m_thermoregulation.onRun(requestAllModules);
    m_threads.add(&m_thermoregulation);

    Thread m_dataCollector = Thread();
    m_dataCollector.setInterval(3000);
    m_dataCollector.onRun(saveMetadataOnSD);
    m_threads.add(&m_dataCollector);
}

void loop() {
    m_threads.run();
}