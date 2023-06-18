#include "ThreadHandler.h"

namespace {
    date_t g_date;
    temperature_t g_tempC;
    light_t g_light;
    thermoreg_t g_thermoreg(eeprom_read_float(ADDRESS_POINT_C),
                            eeprom_read_float(reinterpret_cast<const float *>(ADDRESS_HYSTERESIS)),
                            eeprom_read_float(reinterpret_cast<const float *>(ADDRESS_INERTIA)),
                            eeprom_read_dword(reinterpret_cast<const uint32_t *>(ADDRESS_THREAD_THERMOSTAT)),
                            g_tempC);

    FormatBase* g_formats[4]{&g_date, &g_tempC, &g_thermoreg, &g_light};
    constexpr size_t formatsNum = sizeof(g_formats) / sizeof(g_formats[0]);
    metadata_t<formatsNum> metadata(g_formats);
    SaveHandler saver(metadata.size());

    /// Saves the save state of the previous image in a way to detects
    /// when the SD card is inserted again
    bool isPrevImageSaved = false;
    /// Name of the configuration file
    const char* configFile = "config.bin";
};

namespace {
    /// Save the data of request parameters to the data queue
    void saveMetadataImage() {
        static bool isStackOverflow = false;
        metadata.requestLog |= ((isStackOverflow & !isPrevImageSaved) << ERROR_FILE_QUEUE_OVERFLOW);


        #if (!defined PLOT_MODE && (defined DEBUG_REQUEST_MODE || defined DEBUG_REQUEST_MODE_LOW_MEMORY))
            metadata.toSerial();
            Serial.println();
        #endif

        auto temp = metadata.serialize();
        isStackOverflow = saver.add(temp);
        delete[] temp;
    }

    /// Empty the data stack by recording saved data on an SD card
    void saveMetadataOnSD() {
        if(!(1 & (metadata.requestLog >> INFO_THERMOREGULATION_START)
          || 1 & (metadata.requestLog >> INFO_THERMOREGULATION_END)))
            saveMetadataImage();

        isPrevImageSaved = saver.upload(g_date.getFilename());

        #if (defined DEBUG_THREAD_MODE && !defined PLOT_MODE)
            Serial.println("-> save metadata");
        #endif
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

        #if (defined(DEBUG_THREAD_MODE) && !defined(PLOT_MODE))
           Serial.println("-> request");
        #endif
    }

    /**
     * Setup parameters from the configuration file
     *
     * @retval true if the configuration file could be open
     * @retval false if the configuration file couldn't be open
     */
    bool setupParameters() {
        auto temp = new uint8_t [CONFIG_LENGTH];
        bool state = SaveHandler::readFileBytes(configFile, temp, CONFIG_LENGTH);

        #ifdef DEBUG_CONFIG_DESERIALIZATION
            DataInfo::printRawData(temp, CONFIG_LENGTH);
        #endif

        if(!state) {
            delete[] temp;
            return false;
        }

        auto config = new ConfigHandler(temp);
        config->setupRTC();

        float pointC = 0.0, hysteresis = 0.0, inertia = 0.0;
        config->setupThermostat(pointC, hysteresis, inertia);
        eeprom_update_float(ADDRESS_POINT_C, pointC);
        eeprom_update_float(reinterpret_cast<float *>(ADDRESS_HYSTERESIS), hysteresis);
        eeprom_update_float(reinterpret_cast<float *>(ADDRESS_INERTIA), inertia);

        uint32_t thrdThermostat = 0, thrdSaveData = 0;
        config->setupThreads(thrdThermostat, thrdSaveData);
        eeprom_update_dword(reinterpret_cast<uint32_t *>(ADDRESS_THREAD_THERMOSTAT), thrdThermostat);
        eeprom_update_dword(reinterpret_cast<uint32_t *>(ADDRESS_THREAD_SAVE_DATA), thrdSaveData);

        delete config;
        delete[] temp;
        return true;
    }
};

void ThreadHandler::begin() {
    pinMode(PIN_ENABLE_UPLOAD_CONFIG, INPUT);
    metadata.begin();

    if(digitalRead(PIN_ENABLE_UPLOAD_CONFIG) == HIGH) {
        bool isConfigRead = setupParameters();
        metadata.requestLog |= ((!isConfigRead) << BAD_CONFIG_FILE);
    }
}

void ThreadHandler::run() {
    const uint32_t thrdThermostatInter = eeprom_read_dword(reinterpret_cast<const uint32_t *>(ADDRESS_THREAD_THERMOSTAT));
    const uint32_t thrdSaveDataInter = eeprom_read_dword(reinterpret_cast<const uint32_t *>(ADDRESS_THREAD_SAVE_DATA));

    #ifdef DEBUG_SAVED_CONFIG
        Serial.print("[Saved config]\nThermostat inter: ");
        Serial.println(thrdThermostatInter);
        Serial.print("Save data inter: ");
        Serial.println(thrdSaveDataInter);
    #endif

    while(true) {
        const uint64_t getTime = millis();

        if(uint64_t(getTime % thrdThermostatInter) == 0) {
            requestAllModules();
        }

        #if !isLowMemoryDebugMode()
            if(uint64_t(getTime % thrdSaveDataInter) == 0) {
                saveMetadataOnSD();
            }
        #elif defined DEBUG_REQUEST_MODE_LOW_MEMORY
            if(uint64_t(getTime % thrdSaveDataInter) == 0) {
                saveMetadataImage();
            }
        #endif
    }
}