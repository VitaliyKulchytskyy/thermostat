#pragma once
#include "Macros.h"


/* Debug modes */
/// Prints information about format parameters per request
//#define DEBUG_REQUEST_MODE
/// Build a run time graphic of temperature measurement and the thermoregulation process. Disable other debug modes
//#define PLOT_MODE
/// Debug the threads
//#define DEBUG_THREAD_MODE
/// Debug the way how information is saved on an SD card and on the queue of data
//#define DEBUG_SAVE_HANDLER_MODE
/// Debug the integrity of serialized image
//#define DEBUG_SERIALIZATION
//#define DEBUG_CONFIG_DESERIALIZATION

/* SD card */
/// Pin of chip select
#define SD_CHIP_SELECT          4
/// The value of how many files can be stored without an SD card
#ifdef PLOT_MODE
    #define FILE_QUEUE_SIZE     1
#else
    #define FILE_QUEUE_SIZE     7
#endif


/* Enums, Constants */
/// Temperature module: DS18B20 (Dallas System)
#define TEMP_MODULE_DS18B20     0
/// Temperature module: NTC10K (thermistor)
#define TEMP_MODULE_NTC10K      10000


/* Temperature module (DS18B20 & NTC_10K) */
/// The type of the temperature module to monitoring inside temperature
#define TEMP_TYPE_INSIDE            TEMP_MODULE_DS18B20
/// The type of the temperature module to monitoring outside temperature
#define TEMP_TYPE_OUTSIDE           TEMP_MODULE_NTC10K
/// The pin number connected to the DS18B20 moduleS for TEMP_MODULE_DS18B20 types
#define PIN_ONE_WIRE_BUS            2
/// The pin number connected to the NTC_XK (X - resistance of the module (kOm)) module for TEMP_MODULE_NTCxK types
#define PIN_TEMP_MODULE_NTC         A1
/// Set the temperature precision for the DS18B20 module. Value: 9 - 12.
#define TEMPERATURE_DS_PRECISION    12
/// Set the beta coefficient for inside NTC module
#define BETA_COEFFICIENT_NTC        3950
#define PIVOT_TEMPERATURE           25


/* Thermoregulation settings */
/// The value of hysteresis window
#define HYSTERESIS_C            4.0
/// The value of temperature needs to be kept
#define SET_POINT_C             34
/// The coefficient of system inertia
#define THERMOSTAT_INERTIA      0.5
/// The pin number connected to the relay, which control the pump
#define PIN_RELAY_PUMP          5


/* Threads settings */
/// The baud of the serial port
#define SERIAL_BEGIN_BAUD       9600


/* EEPROM addresses */
#define ADDRESS_POINT_C             0
#define ADDRESS_HYSTERESIS          4
#define ADDRESS_INERTIA             8
#define ADDRESS_THREAD_THERMOSTAT   12
#define ADDRESS_THREAD_SAVE_DATA    16


