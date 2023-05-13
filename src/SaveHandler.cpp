#include "SaveHandler.h"

void printRawData(uint8_t* pRawData, uint8_t formatSize, uint8_t outputSys) {
    Serial.print("Raw format: ");
    for(uint8_t i = 0; i < formatSize; i++) {
        Serial.print(pRawData[i], outputSys);
        Serial.print(" ");
    }

    Serial.println();
}


File SaveHandler::writeBin;

bool SaveHandler::upload(const char *filename) {
    if(!SD.begin(SD_CHIP_SELECT) || m_mdStack->isEmpty())
        return false;

    while (!m_mdStack->isEmpty()) {
        writeBin = SD.open(filename, FILE_WRITE);
        if(!writeBin)
            return false;

        auto temp = new uint8_t [m_rawArraySize];
        m_mdStack->pop(temp);

        writeBin.write(temp, m_rawArraySize);
        writeBin.close();
        delete[] temp;
    }

    return true;
}

bool SaveHandler::add(const void *const pRawData) {
    m_mdStack->push(pRawData);
    return m_mdStack->isFull();
}
