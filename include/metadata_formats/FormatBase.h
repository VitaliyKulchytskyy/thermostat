#pragma once


class FormatBase {
public:
    virtual size_t size() const = 0;

    virtual uint8_t* serialize() const = 0;

    virtual void toSerial() const = 0;
};