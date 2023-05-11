#pragma once
#include "Settings.h"
#include "Metadata.h"
#include "cppQueue.h"
#include "SD.h"

void printRawData(uint8_t* rawFormat, uint8_t formatSize, uint8_t outputSys = HEX) {
    Serial.print("Raw format: ");
    for(uint8_t i = 0; i < formatSize; i++) {
        Serial.print(rawFormat[i], outputSys);
        Serial.print(" ");
    }

    Serial.println();
}

class SaveHandler {
private:
    const size_t m_rawArraySize = 0;
    cppQueue* m_mdStack;
    static File writeBin;
public:
    explicit SaveHandler(size_t rawArraySize)
        : m_rawArraySize(rawArraySize) {
        m_mdStack = new cppQueue(rawArraySize, FILE_STACK_SIZE, FIFO, true);
    }

    ~SaveHandler() {
        delete m_mdStack;
    }
public:
    bool add(const void * const p_raw){
        m_mdStack->push(p_raw);

        return m_mdStack->isFull();
    }

    bool upload(const char* filename) {
        if(!SD.begin(SD_CHIP_SELECT) || m_mdStack->isEmpty())
            return false;

        while (!m_mdStack->isEmpty()) {
            writeBin = SD.open(filename, FILE_WRITE);
            if(!writeBin)
                return false;

            auto temp = new uint8_t [m_rawArraySize];
            m_mdStack->pop(temp);

            Serial.println("-> saved");
            //printRawData(temp, m_rawArraySize);
            writeBin.write(temp, m_rawArraySize);
            writeBin.close();
            delete[] temp;
        }

        return true;
    }
};

File SaveHandler::writeBin;