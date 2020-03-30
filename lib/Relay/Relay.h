#pragma once

#include <Arduino.h>

class Relay
{
private:
    const uint8_t pin;
    const bool inverse;

public:
    Relay(uint8_t pin, bool inverse);

    void setState(bool state)
    {
        digitalWrite(pin, state != inverse);
    }

    bool getState()
    {
        return digitalRead(pin) != inverse;
    }

    void switchState()
    {
        setState(!getState());
    }
};

