#pragma once

#include <Arduino.h>

class Buzzer
{
protected:
    const uint8_t pin;
    const bool inverse;

public:
    Buzzer(uint8_t pin, bool inverse);

    void startBuzzing() {
        digitalWrite(pin, !inverse);
    }

    void stopBuzzing() {
        digitalWrite(pin, inverse);
    }

    void buzz(uint8_t times, uint8_t ms);
};