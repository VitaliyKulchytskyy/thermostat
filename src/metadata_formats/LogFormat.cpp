#include "metadata_formats/LogFormat.h"


#ifdef DEBUG_REQUEST_MODE
void Log::printLogInfo(log_t log, uint8_t outputSys) {
        Serial.print("Log code: ");
        Serial.println(log, outputSys);
    }
#endif