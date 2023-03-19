#pragma once
#include "Arduino.h"

class FormatBase {
public:
    /**
     * Serializes format structure to the .bin file format
     * @return serialized array of bytes
     */
    virtual uint8_t* serialize() const = 0;

    /// Prints information about the format
    virtual void toSerial() const = 0;

    /**
     * @return size of serialized array
     */
    virtual size_t formatSize() const = 0;
};