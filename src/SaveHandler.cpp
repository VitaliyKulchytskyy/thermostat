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
        File* writeFile = new File(SD.open(filename, FILE_WRITE));

        // if(!*writeFile)
        //      return false;

        auto temp = new uint8_t [m_rawArraySize];
        m_mdQueue->pop(temp);

        writeFile->write(temp, m_rawArraySize);
        writeFile->close();
        #if (defined(DEBUG_SAVE_HANDLER_MODE) && !defined(PLOT_MODE))
            Serial.println("--> save an image to the SD");
        #elif (defined(DEBUG_SERIALIZATION) && !defined(PLOT_MODE))
            Serial.println("<- Written on an SD:");
            DataInfo::printRawData(temp, m_rawArraySize);
            Serial.println();
        #endif

        delete[] temp;
        free(writeFile);
    }

    return true;
}

bool SaveHandler::readFileBytes(const char *filename, uint8_t *pBuffer, size_t readBytes) {
    if(!SD.begin(SD_CHIP_SELECT))
        return false;

    File readFile = SD.open(filename, FILE_READ);
    if(!readFile)
        return false;

    readFile.readBytes(pBuffer, readBytes);
    readFile.close();

    return true;
}