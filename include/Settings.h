#pragma once

/* SD card */
/// Pin of chip select
#define SD_CHIP_SELECT          4
/// The value of how many files can be stored without an SD card
#define FILE_STACK_SIZE         4  // 21

/* Temperature module (DS18B20) */
/// The pin number connected to the DS18B20 modules
#define ONE_WIRE_BUS            2
/// Set the temperature precision for the DS18B20 module. Value: 9 - 12.
#define TEMPERATURE_PRECISION   9

/* Threads settings */
/// The interval (in milliseconds) to invoke thermostat checker
#define THREAD_THERMOSTAT_MS    1000
/// The interval (in milliseconds) to invoke saving data on an SD
#define THREAD_SAVE_DATA_SD     3000
/// The baud of the serial port
#define SERIAL_BEGIN_BAUD       9600

/* Thermoregulation settings */
/// The value of hysteresis window
#define HYSTERESIS_C            4.0
/// The value of temperature needs to be kept
#define SET_POINT_C             30
/// The coefficient of system inertia
#define THERMOSTAT_INERTIA      0.5
/// The pin number connected to the relay, which control the pump
#define PUMP_RELAY_PIN          5

/* Other */
/// Debug mode prints information about format parameters per request
#define DEBUG_MODE