#include "ThreadHandler.h"

namespace {
    date_t g_date;
    temperature_t g_tempC;
    light_t g_light;
    thermoreg_f g_thermoreg(g_tempC);

    FormatBase* g_formats[4]{&g_date, &g_tempC, &g_light, &g_thermoreg};
    constexpr size_t formatsNum = sizeof(g_formats) / sizeof(g_formats[0]);
    metadata_t<formatsNum> metadata(g_formats);

    SaveHandler saver(metadata.size());
    ThreadController m_threads = ThreadController();

    /**
     * Saves the save state of the previous image in a way to detects
     * when the SD card is inserted again
     */
    bool isPrevImageSaved = false;
};

namespace {
    /// Save the data of request parameters to the data stack
    void saveMetadataImage() {
        static bool isStackOverflow = false;
        metadata.requestLog |= ((isStackOverflow & !isPrevImageSaved) << ERROR_FILE_STACK_OVERFLOW);

        auto temp = metadata.serialize();
        #ifdef DEBUG_MODE
            metadata.toSerial();
        #endif
        isStackOverflow = saver.add(temp);
        delete[] temp;
    }

    /// Empty the data stack by recording saved data on an SD card
    void saveMetadataOnSD() {
        if(!(1 & (metadata.requestLog >> INFO_THERMOREGULATION_START)
             || 1 & (metadata.requestLog >> INFO_THERMOREGULATION_END)))
            saveMetadataImage();

        isPrevImageSaved = saver.upload(g_date.getFilename());
        Serial.println();
    }

    /**
     * Make request to each modules. If the thermoregulation process started
     * or ended it also save data image at that time
     */
    void requestAllModules() {
        log_t logCode = metadata.request();

        if(1 & (logCode >> INFO_THERMOREGULATION_START)
        || 1 & (logCode >> INFO_THERMOREGULATION_END))
            saveMetadataImage();
    }

    /**
     * Read the file of unique format to set time on the unsettled RTC module.
     * @note The bin format: Hours (0-23), Minutes (0-59), Seconds (0-59),
     * Day (1-31), Month (1-12), Year (23-99).\n
     * In a way to ignore a parameter use \b 0xFF value.
     * @note In a way to reset the RTC module put the battery off and turn down the device for a while
     * @example File format:\n
     * Set only year (d - decimal): 0xFF 0xFF 0xFF 0xFF 0xFF d23;\n
     * Set date (16:14:30 13/05/2023): d16 d14 d30 d13 d5 d23
     * @retval true if the file exists
     * @retval true if the file doesn't exist
     */
    bool readRTCSetupFile() {
        if(!SD.begin(SD_CHIP_SELECT))
            return false;

        File readFile = SD.open("config.bin", FILE_READ);
        if(!readFile)
            return false;

        const size_t dateSize = g_date.size();
        auto buf = new uint8_t[dateSize];
        readFile.readBytes(buf, dateSize);
        readFile.close();

        auto bufDate = g_date.serialize();

        for(size_t i = 0; i < dateSize; i++)
            if(buf[i] == 0xFF)
                buf[i] = bufDate[i];

        date_t::setTime(buf);

        delete[] bufDate;
        delete[] buf;

        return true;
    }
};

void ThreadHandler::begin() {
    metadata.begin();
    log_t logCode = metadata.request();

    if(1 & (logCode >> ERROR_RTC_SET_UP))
        readRTCSetupFile();

    Thread m_thermoregulation = Thread();
    m_thermoregulation.setInterval(THREAD_THERMOSTAT_MS);
    m_thermoregulation.onRun(requestAllModules);
    m_threads.add(&m_thermoregulation);

    Thread m_dataCollector = Thread();
    m_dataCollector.setInterval(THREAD_SAVE_DATA_SD);
    m_dataCollector.onRun(saveMetadataOnSD);
    m_threads.add(&m_dataCollector);
}

void ThreadHandler::run() {
    m_threads.run();
}