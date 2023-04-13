#pragma once
#include "Settings.h"
#include "Metadata.h"
#include "SaveHandler.h"
#include <TimerOne.h>


namespace {
    temperature_t temperature;
    SaveHandler saveHnd;
}

namespace Callback {
    void thermostat();
}

namespace ThreadHandler {
    void begin();
    void run();
    void testSaveSd();
}