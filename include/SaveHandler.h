#pragma once
#include "Arduino.h"
#include "Settings.h"
#include "cppQueue.h"
#include "SD.h"

#if isDebugModeEnable()
namespace DataInfo {
    /**
    * Print the raw data to console
     *
    * @param [in] pRawData pointer to the array of data
    * @param formatSize the number of elements in the array
    * @param outputSys the numerical system to display the data
    */
    void printRawData(uint8_t* pRawData, uint8_t formatSize, uint8_t outputSys = HEX);
};
#endif


/**
 * The class handles saving information on an SD card
 */
class SaveHandler {
public:
    /**
     * Read content from a file and write to a buffer
     *
     * @param filename name of the file
     * @param pBuffer buffer
     * @param readBytes how many bytes read from the file
     * @retval true if a file could be open
     * @retval false if a file couldn't be open
     */
    static bool readFileBytes(const char *filename,
                              uint8_t *pBuffer,
                              size_t readBytes);

public:
    explicit SaveHandler(size_t rawArraySize);

    ~SaveHandler();

public:
    /**
     * Add data to the data stack
     *
     * @param [in] pRawData pointer to the array of data
     * @retval true if queue is overflow
     * @retval false if queue isn't overflow
     */
    bool add(const void * pRawData) __attribute__((nonnull));

    /**
     * Upload all data from the data queue on an SD card
     *
     * @param filename the name of file to save the data
     * @retval true if file was written
     * @retval false if file wasn't written
     */
    bool upload(const char* filename) __attribute__((nonnull));

private:
    const size_t m_rawArraySize = 0;
    cppQueue* m_mdQueue;
};