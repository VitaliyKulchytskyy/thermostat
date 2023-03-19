#include <Arduino.h>
#include "ThreadHandler.h"


void setup() {
    Serial.begin(9600);
    ThreadHandler::begin();
}

void loop() {
    ThreadHandler::run();
}