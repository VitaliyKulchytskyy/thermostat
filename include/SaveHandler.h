#pragma once
#include "Arduino.h"
#include "Settings.h"
#include "Metadata.h"
#include "cppQueue.h"
#include "SD.h"


class SaveHandler {
private:
    cppQueue m_mdStack{sizeof(Metadata), FILE_STACK_SIZE, FIFO};
    uint8_t m_count = 0;
public:
    SaveHandler() = default;
public:
    bool add(Metadata);
    bool unload();
};