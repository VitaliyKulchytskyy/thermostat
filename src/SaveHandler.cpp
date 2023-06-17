#include "SaveHandler.h"

#if isDebugModeEnable()
void DataInfo::printRawData(uint8_t *pRawData, uint8_t formatSize, uint8_t outputSys) {
    Serial.print("Raw format: ");
    for(uint8_t i = 0; i < formatSize; i++) {
        Serial.print(pRawData[i], outputSys);
        Serial.print(" ");
    }

    Serial.println();
}
#endif


File  SaveHandler::m_binFile;

SaveHandler::SaveHandler(size_t rawArraySize)
        : m_rawArraySize(rawArraySize),
          m_mdQueue{new cppQueue(rawArraySize,
                                 FILE_QUEUE_SIZE,
                                 FIFO,
                                 true)}
{}

SaveHandler::~SaveHandler() {
    delete m_mdQueue;
}

bool SaveHandler::add(const void *const pRawData) {
    m_mdQueue->push(pRawData);
    const bool isQueueFull = m_mdQueue->isFull();

    #if (defined(DEBUG_SAVE_HANDLER_MODE) && !defined(PLOT_MODE))
        Serial.println("--> add an image to queue");
        if(isQueueFull)
            Serial.println("---> the queue is full!");
    #elif (defined(DEBUG_SERIALIZATION) && !defined(PLOT_MODE))
        Serial.println("<- add to data queue");
        DataInfo::printRawData((uint8_t*)pRawData, m_rawArraySize);
        Serial.println();
    #endif

    return isQueueFull;
}

bool SaveHandler::upload(const char *filename) {
    if(!SD.begin(SD_CHIP_SELECT) || m_mdQueue->isEmpty())
        return false;

    while (!m_mdQueue->isEmpty()) {
        m_binFile = SD.open(filename, FILE_WRITE);

        // Біс його знає чому при взаємодії з BH1750 відкритий файл думає,
        // що він закритий, хоч він нормально записується без перевірки нижче
        // if(!m_binFile)
        //     return false;

        auto temp = new uint8_t [m_rawArraySize];
        m_mdQueue->pop(temp);

        m_binFile.write(temp, m_rawArraySize);
        m_binFile.close();

    #if (defined(DEBUG_SAVE_HANDLER_MODE) && !defined(PLOT_MODE))
        Serial.println("--> save an image to the SD");
    #elif (defined(DEBUG_SERIALIZATION) && !defined(PLOT_MODE))
        Serial.println("<- Written on an SD:");
        DataInfo::printRawData(temp, m_rawArraySize);
        Serial.println();
    #endif

        delete[] temp;
    }

    return true;
}

bool SaveHandler::readFile(const char *filename, uint8_t *&rpRawFile, size_t readBytes, size_t offset) {
    m_binFile = SD.open(filename, FILE_READ);
    if(!m_binFile)
        return false;

    m_binFile.readBytes(&rpRawFile[offset], readBytes - offset);

    m_binFile.close();
    return true;
}
