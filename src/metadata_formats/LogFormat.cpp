#include "metadata_formats/LogFormat.h"


#if (defined DEBUG_REQUEST_MODE || defined DEBUG_REQUEST_MODE_LOW_MEMORY)
void Log::printLogInfo(log_t log, uint8_t outputSys) {
        Serial.print("Log code: ");
        Serial.println(log, outputSys);
    }
#endif