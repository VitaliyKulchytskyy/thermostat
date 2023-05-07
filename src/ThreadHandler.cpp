/*#include "ThreadHandler.h"

void ThreadHandler::begin() {
    Timer1.initialize(100000);
    Timer1.attachInterrupt(Callback::thermostat);
    date_t::begin();
}

void Callback::thermostat() {
    temperature = temperature_t::readModuleInstance();
    Serial.println("-> read temperature modules");
}*/
/*
void ThreadHandler::testSaveSd() {
    date_t date = date_t::readModuleInstance();
    metadata_t mdt{date, temperature_t::readModuleInstance(), 0x00};
    saveHnd.add(mdt);
    saveHnd.upload();
}*/
