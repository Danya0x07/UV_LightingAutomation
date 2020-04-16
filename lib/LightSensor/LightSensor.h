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
    explicit LightSensor(uint8_t pin_): pin(pin_) {}
    uint16_t getValue(uint16_t limit);
};
