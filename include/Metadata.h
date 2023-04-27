#pragma once
/*#include "metadata_formats/DateFormat.h"
#include "metadata_formats/LogFormat.h"
#include "metadata_formats/TemperatureFormat.h"
#include "metadata_formats/FormatBase.h"


*//**
 * This class provides a proper way to save formats (DateFormat, TemperatureFormat, LogFormat etc) on the SD card in the .bin file
 * Files can be saved without emptying an SD card for one year. Either older files will be rewritten by newer (FAT16 problem).
 *//*
struct metadata_t: public FormatBase {
public:
    //uint8_t logs;
    FormatBase* test_[2]{new date_t(), new temperature_t()};
public:
    static const char* getFilename() {
        static char filename[16]{};
        sprintf(filename, "%02i%02i20%02i.bin",
                date_t::day,
                date_t::month,
                date_t::year);

        return filename;
    }

    size_t size() const override {
        size_t output = 0;
        for(const auto& i: test_)
            output += i->size();

        return output;
    }

    uint8_t *serialize() const override {
        return FormatBase::serialize();
    }

    void toSerial() const override {
        for(const auto& i: test_)
            i->toSerial();
    }
};*/
