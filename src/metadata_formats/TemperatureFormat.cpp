#include "Arduino.h"
#include "metadata_formats/TemperatureFormat.h"


TemperatureFormat::TemperatureFormat() {
    InsideTemp = analogRead(INSIDE_THERMOMETER_PIN);
    OutsideTemp = analogRead(OUTSIDE_THERMOMETER_PIN);
}

uint8_t* TemperatureFormat::serialize() const {
    auto* output = new uint8_t[FORMAT_VOLUME]{};

    output[INSIDE_TEMP_SHIFT] = InsideTemp;
    output[OUTSIDE_TEMP_SHIFT] = OutsideTemp;

    return output;
}

void TemperatureFormat::toSerial() const {
    Serial.print("InsideTemp: ");
    Serial.print(InsideTemp);
    Serial.print("C | OutsideTemp: ");
    Serial.print(InsideTemp);
    Serial.println("C");
}

size_t TemperatureFormat::formatSize() const {
    return FORMAT_VOLUME;
}
