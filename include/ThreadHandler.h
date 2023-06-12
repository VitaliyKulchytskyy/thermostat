#pragma once
#include "Metadata.h"
#include "SaveHandler.h"

namespace ThreadHandler {
    /// Setup the device
    void begin();

    /// Run the device
    void run() __attribute__((noreturn));
};