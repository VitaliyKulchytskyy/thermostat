#pragma once
#include "Metadata.h"
#include "SaveHandler.h"
#include "ConfigHandler.h"
#include <avr/eeprom.h>

namespace ThreadHandler {
    /// Setup the device
    void begin();

    /// Run the device
    void run() __attribute__((noreturn));
};