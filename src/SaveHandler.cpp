#include "SaveHandler.h"


bool SaveHandler::add(Metadata md) {
    m_count++;
    bool status = m_mdStack.push(&md);
    return status;
}

bool SaveHandler::unload() {
    if(!SD.begin(SD_CHIP_SELECT) || !m_mdStack.isEmpty())
        return false;

    do {
        Metadata* md;
        m_mdStack.pop(md);
        md->toSerial();
    } while(--m_count > 0);

    return true;
}

