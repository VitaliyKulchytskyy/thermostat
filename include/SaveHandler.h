#pragma once
#include "Arduino.h"
#include "Settings.h"
#include "cppQueue.h"
#include "SD.h"

#ifdef DEBUG_MODE
    /**
     * Print the raw data to console
     * @param [in] pRawData pointer to the array of data
     * @param formatSize the number of elements in array
     * @param outputSys the numerical system to display the data
     */
    void printRawData(uint8_t* pRawData, uint8_t formatSize, uint8_t outputSys = HEX);
#endif

/**
 * The class handles saving information on an SD card
 */
class SaveHandler {
public:
    explicit SaveHandler(size_t rawArraySize)
        : m_rawArraySize(rawArraySize) {
        m_mdStack = new cppQueue(rawArraySize, FILE_STACK_SIZE, FIFO, true);
    }

    ~SaveHandler() {
        delete m_mdStack;
    }
public:
    /**
     * Add data to the data stack
     * @param [in] pRawData pointer to the array of data
     * @retval true if stack is overflow
     * @retval false if stack isn't overflow
     */
    bool add(const void * pRawData);

    /**
     * Upload all data from the data stack on an SD card
     * @param filename the name of file to save the data
     * @retval true if file was written
     * @retval false if file wasn't written
     */
    bool upload(const char* filename);

private:
    static File writeBin;

private:
    const size_t m_rawArraySize = 0;
    cppQueue* m_mdStack;
};