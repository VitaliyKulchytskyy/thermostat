#include <Arduino.h>
#include <Thread.h>
#include <ThreadController.h>
#include "Metadata.h"
#include "SaveHandler.h"

#define THREAD_THERMOSTAT_MS    1000
#define HYSTERESIS_C            4.0
#define SET_POINT_C             30
#define THERMOSTAT_INERTIA      0.5
#define RELAY_PIN               5

/*
 * TODO: Implement Metadata.h [+]
 *      TODO: Implement serialization on SD [+]
 * TODO: Implement logs [+]
 *      TODO: Implement all error codes [+]
 * TODO: Implement watch backup
 * TODO: Implement pseudo-threads [+]
 * TODO: Implement thermoregulation [+]
 *      TODO: Код повернення скидається в 0 раніше запланованого [+]
 * TODO: Define DEBUG
 *      TODO: Cover over DEBUG flag all toSerial() methods (also in the interface definition)
 *      TODO: Cover over DEBUG flag all Serial outputs
 * TODO: Rewrite size() in Metadata.h and in Metadata.serialize()
 * TODO: Find out where defines should be placed
 * TODO: заміряти коли певні ділянки коду працюють швидке чи повільніше
 */

date_t g_date;
temperature_t g_tempC;
light_t g_light;
thermoreg_f g_thermoreg(g_tempC);

FormatBase* g_formats[4]{&g_date, &g_tempC, &g_light, &g_thermoreg};
constexpr size_t formatsNum = sizeof(g_formats) / sizeof(g_formats[0]);
metadata_t<formatsNum> metadata(g_formats);

SaveHandler saver(metadata.size());

ThreadController m_threads = ThreadController();

bool isPrevImageSaved = false;
void saveMetadataImage() {
    static bool isStackOverflow = false;
    metadata.requestLog |= ((isStackOverflow & !isPrevImageSaved) << ERROR_FILE_STACK_OVERFLOW);

    auto temp = metadata.serialize();
    metadata.toSerial();
    isStackOverflow = saver.add(temp);
    delete[] temp;
}

void saveMetadataOnSD() {
    if(!(1 & (metadata.requestLog >> INFO_THERMOREGULATION_START)
      || 1 & (metadata.requestLog >> INFO_THERMOREGULATION_END)))
        saveMetadataImage();

    isPrevImageSaved = saver.upload(g_date.getFilename());
    Serial.println();
}

void requestAllModules() {
    log_t returnCode = metadata.request();

    if(1 & (returnCode >> INFO_THERMOREGULATION_START)
    || 1 & (returnCode >> INFO_THERMOREGULATION_END))
        saveMetadataImage();
}

/*
 * # Файл для задання дати на RTC модулі.
 * #
 * # Змінюйте значення змінних відповідно до параметра:
 * # Назва_параметру=цілочисельне_значення
 * #
 * # Для того, щоб прицільно змінити певний параметр
 * # закоментуйте ті параметри, які не потребують зміни.
 * # Для того, щоб закоментувати рядок використовуйте символ - #.
 * #
 * # Задання години (від 0 до 23):
 * Hours=12
 * # Задання хвилини (від 0 до 59):
 * Minutes=30
 * # Задання секунд (від 0 до 59):
 * Seconds=0
 * # Задання дня (від 1 до 31):
 * Day=15
 * # Задання місяця (від 1 до 12):
 * Month=5
 * # Задання року (від 23 до 99):
 * Year=23
 */
uint8_t* getParsedRTCFile(const char* filename) {
    if(!SD.begin(SD_CHIP_SELECT))
        return nullptr;

    File readFile = SD.open(filename, FILE_READ);
    if(!readFile)
        return nullptr;

    while(readFile.available()) {

    }
}

void setup() {
    Serial.begin(9600);
    metadata.begin();
    //date_t::setTime();

    Thread m_thermoregulation = Thread();
    m_thermoregulation.setInterval(THREAD_THERMOSTAT_MS);
    m_thermoregulation.onRun(requestAllModules);
    m_threads.add(&m_thermoregulation);

    Thread m_dataCollector = Thread();
    m_dataCollector.setInterval(3000);
    m_dataCollector.onRun(saveMetadataOnSD);
    m_threads.add(&m_dataCollector);
}

void loop() {
    m_threads.run();
}