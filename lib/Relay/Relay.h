#pragma once

#include <Arduino.h>

/**
 * Обёртка над реле для инкапсуляции инициализации пинов
 * и удобного переключения с учётом возможной инверсности управления.
 */
class Relay
{
private:
    const uint8_t pin;
    const bool inverted;

public:
    explicit Relay(uint8_t pin, bool inverted);

    void setState(bool state)
    {
        digitalWrite(pin, state != inverted);
    }

    bool getState()
    {
        return bool(digitalRead(pin)) != inverted;
    }

    void switchState()
    {
        setState(!getState());
    }
};

