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
    cppQueue m_mdStack{sizeof(uint8_t*), FILE_STACK_SIZE, FIFO, true};
    uint8_t m_count = 0;
    const size_t m_rawArraySize = 0;
    static File writeBin;
public:
    explicit SaveHandler(size_t rawArraySize)
        : m_rawArraySize(rawArraySize) {}
public:
    bool add(uint8_t* p_raw){
        if(m_count < FILE_STACK_SIZE)
            m_count++;

        m_mdStack.push(p_raw);

        return m_mdStack.isFull();
    }

    bool upload(const char* filename) {
        if(!SD.begin(SD_CHIP_SELECT) || m_mdStack.isEmpty())
            return false;

        do {
            writeBin = SD.open(filename, FILE_WRITE);
            if(!writeBin)
                return false;

            uint8_t* temp;
            m_mdStack.pop(&temp);

            writeBin.write(temp, m_rawArraySize);
            writeBin.close();
        } while (--m_count > 0);

        return true;
    }
};

File SaveHandler::writeBin;