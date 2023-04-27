#include "Arduino.h"
#include "metadata_formats/DateFormat.h"

iarduino_RTC date_t::_clock = RTC_DS3231;

size_t date_t::size() const {
    return 6;
}

uint8_t *date_t::serialize() const {
    return FormatBase::serialize();
}
