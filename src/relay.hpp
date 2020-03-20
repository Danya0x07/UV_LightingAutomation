#pragma once
#include <Arduino.h>

class Relay
{
private:
    const uint8_t pin;
    const bool inverse;

public:
    Relay(uint8_t p, bool inv = false)
        : pin(p), inverse(inv)
    {
        pinMode(pin, OUTPUT);
    }

    void set_state(bool state)
    {
        digitalWrite(pin, state != inverse);
    }

    bool get_state()
    {
        return digitalRead(pin) != inverse;
    }

    void switch_state()
    {
        digitalWrite(pin, !digitalRead(pin));
    }
};

