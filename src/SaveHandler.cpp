#include "SaveHandler.h"

File SaveHandler::writeBin;


bool SaveHandler::add(metadata_t mdt) {
    m_count++;
    return m_mdStack.push(&mdt);
}

bool SaveHandler::unload() {
    if(!SD.begin(SD_CHIP_SELECT) || m_mdStack.isEmpty())
        return false;

    do {
        metadata_t mdt{};
        m_mdStack.pop(&mdt);

        writeBin = SD.open(mdt.getFilename(), FILE_WRITE);
        if(!writeBin)
            return false;

        auto *bytes = new uint8_t[m_mdtSize]{};
        memcpy(bytes, &mdt, m_mdtSize);
        writeBin.write(bytes, m_mdtSize);
        delete[] bytes;

        writeBin.close();
    } while (--m_count > 0);
    Serial.println("-> data was saved");

    return true;
}

