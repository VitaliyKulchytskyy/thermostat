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
    }
}