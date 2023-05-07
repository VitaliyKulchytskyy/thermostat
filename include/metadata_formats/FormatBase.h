#pragma once

class FormatBase {
public:
    virtual size_t size() const = 0;

    virtual uint8_t* serialize() const = 0;

    virtual void toSerial() const = 0;

    virtual log_t request() = 0;

    virtual void afterRequest() = 0;

    virtual void begin() = 0;
};