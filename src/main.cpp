#include <Arduino.h>
#include <TimerOne.h>
#include "Metadata.h"
#include "SaveHandler.h"

/*
 * TODO: Implement Metadata.h [+]
 *      TODO: Implement serialization on SD [+]
 * TODO: Implement logs [+]
 * TODO: Implement watch backup
 * TODO: Implement pseudo-threads
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
    metadata.toSerial();
    //printRawData(metadata);

    auto temp = metadata.serialize();
    //printRawData(temp, mdtSize);
    isStackOverflow = saver.add(temp);
    isSaved = saver.unload(g_date.getFilename());
    delete[] temp;
}

void setup()
{
    Serial.begin(9600);
    metadata.begin();

}

void loop()
{
    saveMetadataOnSD();
    delay(1000);
}