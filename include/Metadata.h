#pragma once
#include "metadata_formats/FormatBase.h"
#include "metadata_formats/DateFormat.h"
#include "metadata_formats/LogFormat.h"
#include "metadata_formats/TemperatureFormat.h"


/**
 * This class provides a proper way to save formats (DateFormat, TemperatureFormat, LogFormat) on the SD card in the .bin file
 * Files can be saved without emptying an SD card for one year. Either older files will be rewritten by newer (FAT16 problem).
 */
class Metadata final: FormatBase {
private:
    //const DateFormat& m_date;
    //const TemperatureFormat& m_temp;
    //const LogFormat& m_log;
public:
    Metadata() = default;
    //Metadata(const DateFormat &mDate, const TemperatureFormat &mTemp, const LogFormat &mLog);
public:
    uint8_t *serialize() const override;

    void toSerial() const override;

    size_t formatSize() const override;
public:
    /*
    DateFormat getDateFormat() const {
        return m_date;
    }

    TemperatureFormat getTemperatureFormat() const {
        return m_temp;
    }

    LogFormat getLogFormat() const {
        return m_log;
    }
     */

    const char* filename() const {
        /*
        static char record[16]{};
        auto rtc = getDateFormat();

        sprintf(record, "%02i%02i%02i%02i.bin",
                rtc.Month,
                rtc.Day,
                rtc.Hours,
                rtc.Minutes);
    */
        return "record.bin";
    }
};
