#pragma once

/* Pins */
/// Analog pin of the outside thermometer module
#define OUTSIDE_THERMOMETER_PIN     A0
/// Analog pin of the inside thermometer module
#define INSIDE_THERMOMETER_PIN      A1

/* Threads */
/// Interval of calling the thermostat procedure
#define THREAD_THERMOSTAT_INTERVAL_MS         500
/// Interval of calling the data collector procedure
#define THREAD_DATA_COLLECTOR_INTERVAL_MS     2500

/* SD card */
/// Pin of chip select
#define SD_CHIP_SELECT          4
/// The value of how many files can be stored without an SD card
#define FILE_STACK_SIZE         21