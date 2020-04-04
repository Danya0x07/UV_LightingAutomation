#pragma once

#include <Arduino.h>

/**
 * Обёртка над пьезопищалкой для инкапсуляции инициализации пинов
 * и удобного переключения с учётом возможной инверсности управления.
 */
class Buzzer
{
private:
    const uint8_t pin;
    const bool inverted;

public:
    explicit Buzzer(uint8_t pin, bool inverted);

    void startBuzzing() {
        digitalWrite(pin, !inverted);
    }

    void stopBuzzing() {
        digitalWrite(pin, inverted);
    }

    void buzz(uint8_t times, uint8_t ms);
};