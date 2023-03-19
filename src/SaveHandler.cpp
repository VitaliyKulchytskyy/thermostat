#include "SaveHandler.h"


bool SaveHandler::add(const Metadata& md) {
    Serial.println("here");
    return m_mdStack.push(&md);
}

bool SaveHandler::unload() {
    if(!SD.begin(SD_CHIP_SELECT))
        return false;

    Metadata md;
    m_mdStack.pop(&md);
    Serial.println(md.filename());

    //Serial.println("END----------");

    return true;
}

