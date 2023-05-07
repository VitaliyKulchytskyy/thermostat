#pragma once

/// Flag of incorrect INSIDE thermometer connection
#define BAD_REQUEST_THERMOMETER_INSIDE      0
/// Flag of incorrect OUTSIDE thermometer connection
#define BAD_REQUEST_THERMOMETER_OUTSIDE     1
/// Flag of incorrect RTC set up
#define ERROR_RTC_SET_UP                    2
/// Flag of absence uninterrupted RTC supplying
#define WARNING_RTC_UNIRQ_POWER             3
/// Flag of stack overflow. Losing the data
#define ERROR_FILE_STACK_OVERFLOW           4
/// Flag of coursing of the thermoregulation process
#define INFO_THERMOREGULATION_PROCESS       5
/// Flag of starting of the thermoregulation process
#define INFO_THERMOREGULATION_START         6
/// Flag of ending of the thermoregulation process
#define INFO_THERMOREGULATION_END           7

using log_t = uint8_t;

namespace Log {
    void printLogInfo(log_t log, uint8_t outputSys = BIN) {
        Serial.print("Error code: ");
        Serial.println(log, outputSys);
/*        Serial.println();
        Serial.print("Bad requests to INSIDE thermometer:\t\t");
        Serial.println(isBitSet(log, BAD_REQUEST_THERMOMETER_INSIDE));
        Serial.print("Bad requests to OUTSIDE thermometer:\t\t");
        Serial.println(isBitSet(log, BAD_REQUEST_THERMOMETER_INSIDE));
        Serial.print("RTC wasn't set up:\t\t\t\t");
        Serial.println(isBitSet(log, ERROR_RTC_SET_UP));
        Serial.print("There is no uninterrupted power supply:\t\t");
        Serial.println(isBitSet(log, WARNING_RTC_UNIRQ_POWER));
        Serial.print("The data was written from the stack:\t\t");
        Serial.println(isBitSet(log, ERROR_FILE_STACK_OVERFLOW));
        Serial.print("The thermoregulation process is running:\t");
        Serial.println(isBitSet(log, INFO_THERMOREGULATION_PROCESS));
        Serial.print("The thermoregulation process was started:\t");
        Serial.println(isBitSet(log, INFO_THERMOREGULATION_START));
        Serial.print("The thermoregulation process was finish:\t");
        Serial.println(isBitSet(log, INFO_THERMOREGULATION_END));*/
    }
}