#pragma once
#include "Arduino.h"
#include "Settings.h"
#include "Metadata.h"
#include "cppQueue.h"
#include "SD.h"


class SaveHandler {
private:
    cppQueue m_mdStack{sizeof(metadata_t), FILE_STACK_SIZE, FIFO};
    uint8_t m_count = 0;
    const size_t m_mdtSize = sizeof(metadata_t);
    static File writeBin;
public:
    SaveHandler() = default;
public:
    bool add(metadata_t);
    bool unload();
};