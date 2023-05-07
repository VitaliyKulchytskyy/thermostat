#pragma once

struct sync_t: public FormatBase {
public:
    log_t syncErrorCode = 0;
public:
    size_t size() const override {
        return 0;
    }

    uint8_t *serialize() const override {
        return nullptr;
    }

    void toSerial() const override {
        Serial.print("Thread sync error: ");
        Serial.println(syncErrorCode, BIN);
    }

    log_t request() override {
        return this->syncErrorCode;
    }

/*    void afterRequest() override {
        syncErrorCode = 0;
    }*/

    void begin() override {}
};