#pragma once

#include <Arduino.h>

class LightSensor
{
protected:
    const uint8_t pin;

public:
    LightSensor(uint8_t pin): pin(pin) {}
    uint16_t getValue(uint16_t limit = 100);
};