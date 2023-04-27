#include "Arduino.h"
#include "metadata_formats/TemperatureFormat.h"

/*
 * TODO: Add tick service
 * TODO: Implement serialization on SD
 * TODO: Implement thermoregulation
 */

temperature_t temp;

void printRawData(FormatBase& format) {
    uint8_t *rawFormat = format.serialize();
    const uint8_t formatSize = format.size();

    Serial.print("Raw format: ");
    for(uint8_t i = 0; i < formatSize; i++) {
        Serial.print(rawFormat[i], HEX);
        Serial.print(" ");
    }

    Serial.println();
    delete[] rawFormat;
}

void setup(void)
{
    // start serial port
    Serial.begin(9600);
}

void loop(void)
{
    auto error = temp.tick();
    temp.toSerial();
    Serial.print("Error code: 0b");
    Serial.print(error, BIN);
    Serial.println();
    printRawData(temp);
}