//#include "SaveHandler.h"
/*
File SaveHandler::writeBin;


bool SaveHandler::add(metadata_t mdt) {
    m_count++;
    return m_mdStack.push(&mdt);
}

bool SaveHandler::unload() {
    if(!SD.begin(SD_CHIP_SELECT) || m_mdStack.isEmpty())
        return false;

    do {
        metadata_t mdt;
        m_mdStack.pop(&mdt);
        const uint8_t mdtSize = mdt.size();

        writeBin = SD.open(mdt.getDateFormat().getFilename(), FILE_WRITE);
        if(!writeBin)
            return false;

        auto bytes = new uint8_t[mdtSize]{};
        auto mdtRaw = mdt.serialize();

        memcpy(bytes, mdtRaw, mdtSize);
        writeBin.write(bytes, mdtSize);

        delete[] mdtRaw;
        delete[] bytes;
        writeBin.close();
    } while (--m_count > 0);
    //Serial.println("-> data was saved");

    return true;
}*/

