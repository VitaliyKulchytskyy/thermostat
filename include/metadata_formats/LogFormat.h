#pragma once
#include "Arduino.h"
#include "Settings.h"

/// Flag of incorrect INSIDE thermometer connection
#define BAD_REQUEST_THERMOMETER_INSIDE      0
/// Flag of incorrect OUTSIDE thermometer connection
#define BAD_REQUEST_THERMOMETER_OUTSIDE     1
/// Flag of incorrect RTC set up
#define ERROR_RTC_SET_UP                    2
/// Flag of incorrect config file
#define BAD_CONFIG_FILE                     3
/// Flag of queue overflow. Losing the data
#define ERROR_FILE_QUEUE_OVERFLOW           4
/// Flag of coursing of the thermoregulation process
#define INFO_THERMOREGULATION_PROCESS       5
/// Flag of starting of the thermoregulation process
#define INFO_THERMOREGULATION_START         6
/// Flag of ending of the thermoregulation process
#define INFO_THERMOREGULATION_END           7

/// The alias to represent log code
using log_t = uint8_t;

namespace Log {
#if (defined DEBUG_REQUEST_MODE || defined DEBUG_REQUEST_MODE_LOW_MEMORY)
    /**
     * Prints the return log code of the request
     * @param log log code
     * @param outputSys the numerical system to display the log
     */
    void printLogInfo(log_t log, uint8_t outputSys = BIN);
#endif
}