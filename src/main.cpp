#include "Arduino.h"
#include "metadata_formats/TemperatureFormat.h"

temperature_t temp;

void setup(void)
{
    // start serial port
    Serial.begin(9600);
}

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

void loop(void)
{
    auto error = temp.tick();
    temp.toSerial();
    Serial.print("Error code: 0b");
    Serial.print(error, BIN);
    Serial.println();
    printRawData(temp);
}