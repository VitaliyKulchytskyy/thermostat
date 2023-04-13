#pragma once
#include "metadata_formats/DateFormat.h"
#include "metadata_formats/LogFormat.h"
#include "metadata_formats/TemperatureFormat.h"


/**
 * This class provides a proper way to save formats (DateFormat, TemperatureFormat, LogFormat) on the SD card in the .bin file
 * Files can be saved without emptying an SD card for one year. Either older files will be rewritten by newer (FAT16 problem).
 */
struct metadata_t {
public:
    date_t date;
    temperature_t temperature;
    uint8_t logs;
public:
    void toSerial() const {
        Serial.println("Date:");
        date.toSerial();
        Serial.println("Temperatures:");
        temperature.toSerial();
        Serial.print("Log: ");
        Serial.println(logs, HEX);
        Serial.println("==============");
    }

    const char* getFilename() const {
        static char filename[16]{};
        sprintf(filename, "%02i%02i20%02i.bin",
                date.day,
                date.month,
                date.year);

        return filename;
    }
};
