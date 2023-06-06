#pragma once


class FormatBase {
public:
    /// Initialize the format
    virtual void begin() = 0;

    /// Gets the size of the format
    virtual size_t size() const = 0;

    /// Gets the serialized array of the format
    virtual uint8_t* serialize() const = 0;

    /// Makes a request to update the format's values
    virtual log_t request() = 0;

#ifdef DEBUG_REQUEST_MODE
    /// Prints an information about format's values
    virtual void toSerial() const = 0;
#endif
};