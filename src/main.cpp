#include <Arduino.h>
#include "ThreadHandler.h"

/*
 * TODO:
 *  [+] Fix stack push & pop
 *  [-] Serialize metadata on the SD card
 *  [-] Relay algorithm for thermoregulation
 */


void testSD() {
    Sd2Card card;
    SdVolume volume;
    SdFile root;

    Serial.begin(9600);
    while (!Serial);

    Serial.print("\nInitializing SD card...");

    if (!card.init(SPI_HALF_SPEED, SD_CHIP_SELECT)) {
        Serial.println("initialization failed. Things to check:");
        Serial.println("* is a card inserted?");
        Serial.println("* is your wiring correct?");
        Serial.println("* did you change the chipSelect pin to match your shield or module?");
        Serial.println("Note: press reset button on the board and reopen this Serial Monitor after fixing your issue!");
        while (1);
    } else {
        Serial.println("Wiring is correct and a card is present.");
    }

    Serial.println();
    Serial.print("Card type:         ");
    switch (card.type()) {
        case SD_CARD_TYPE_SD1:
            Serial.println("SD1");
            break;
        case SD_CARD_TYPE_SD2:
            Serial.println("SD2");
            break;
        case SD_CARD_TYPE_SDHC:
            Serial.println("SDHC");
            break;
        default:
            Serial.println("Unknown");
    }

    if (!volume.init(card)) {
        Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
        while (1);
    }

    Serial.print("Clusters:          ");
    Serial.println(volume.clusterCount());
    Serial.print("Blocks x Cluster:  ");
    Serial.println(volume.blocksPerCluster());

    Serial.print("Total Blocks:      ");
    Serial.println(volume.blocksPerCluster() * volume.clusterCount());
    Serial.println();

    uint32_t volumesize;
    Serial.print("Volume type is:    FAT");
    Serial.println(volume.fatType(), DEC);

    volumesize = volume.blocksPerCluster();
    volumesize *= volume.clusterCount();
    volumesize /= 2;
    Serial.print("Volume size (KB):  ");
    Serial.println(volumesize);
    Serial.print("Volume size (MB):  ");
    volumesize /= 1024;
    Serial.println(volumesize);
    Serial.print("Volume size (GB):  ");
    Serial.println((float)volumesize / 1024.0);

    Serial.println("\nFiles found on the card (name, date and size in bytes): ");
    root.openRoot(volume);

    root.ls(LS_R | LS_DATE | LS_SIZE);
    root.close();
}

metadata_t test;

void setup() {
    Serial.begin(9600);
    if(!SD.begin(SD_CHIP_SELECT))
        Serial.println("SD card is unable");
    date_t::begin();
    //ThreadHandler::begin();
    //date_t::setTime();
}

void loop() {
    //date_t test = date_t::readModuleInstance();
    //test.toSerial();
    test.toSerial();
    delay(1000);
    //delay(2000);
    //ThreadHandler::testSaveSd();
}



