#pragma once

/// Checks whether one of the debug mode is enable
#define isDebugModeEnable() (defined DEBUG_REQUEST_MODE     \
            || defined PLOT_MODE                            \
            || defined DEBUG_THREAD_MODE                    \
            || defined DEBUG_SAVE_HANDLER_MODE              \
            || defined DEBUG_SERIALIZATION)

#define isNTCModule(tempType) ((tempType) == TEMP_MODULE_NTC10K)

#define isDSModule(tempType) ((tempType) == TEMP_MODULE_DS18B20)