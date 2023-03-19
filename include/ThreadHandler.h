#pragma once
#include "Settings.h"
#include "Metadata.h"
#include "ThreadController.h"
#include "Thread.h"
#include "SaveHandler.h"


namespace {
    ThreadController m_threads = ThreadController();
    Thread m_thermoregulation = Thread();
    Thread m_dataCollector = Thread();

    TemperatureFormat temperature;
    LogFormat m_log;
    SaveHandler saveHnd;
}

namespace {
    void save(const Metadata&);
}

namespace Callback {
    void dataCollector();
    void thermostat();
}

namespace ThreadHandler {
    void begin();
    void run();
}