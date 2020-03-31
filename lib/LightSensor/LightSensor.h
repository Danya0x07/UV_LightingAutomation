#pragma once

#include <Arduino.h>

/**
 * Обёртка над аналоговым датчиком освещённости
 * для удобного считывания значения в нужной размерности 
 * (по умолчанию 0-100%).
 */
class LightSensor
{
private:
    const uint8_t pin;

public:
    LightSensor(uint8_t pin): pin(pin) {}
    uint16_t getValue(uint16_t limit = 100);
};