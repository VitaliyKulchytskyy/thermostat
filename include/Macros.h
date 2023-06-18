#pragma once

/// Checks whether one of the debug mode is enable
#define isDebugModeEnable() (defined DEBUG_REQUEST_MODE     \
            || defined DEBUG_REQUEST_MODE_LOW_MEMORY        \
            || defined PLOT_MODE                            \
            || defined DEBUG_THREAD_MODE                    \
            || defined DEBUG_SAVE_HANDLER_MODE              \
            || defined DEBUG_SERIALIZATION                  \
            || defined DEBUG_CONFIG_DESERIALIZATION         \
            || defined DEBUG_FREEMODE                       \
            || defined DEBUG_FREEMODE_LOW_MEMORY            \
            || defined DEBUG_SAVED_CONFIG)

#define isNTCModule(tempType) ((tempType) == TEMP_MODULE_NTC10K)

#define isDSModule(tempType) ((tempType) == TEMP_MODULE_DS18B20)

/// Checks whether one of the enabled debug mode is tagged as LOW MEMORY
#define isLowMemoryDebugMode() (defined PLOT_MODE           \
            || defined DEBUG_CONFIG_DESERIALIZATION         \
            || defined DEBUG_FREEMODE_LOW_MEMORY            \
            || defined DEBUG_REQUEST_MODE_LOW_MEMORY        \
            || defined DEBUG_SAVED_CONFIG)