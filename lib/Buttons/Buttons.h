#pragma once

#include <Arduino.h>

class Button
{
protected:
    const uint8_t pin;
    const bool inverse;
    bool lastState;

public:
    Button(uint8_t pin, bool inverse);
    bool isPressed();
    bool hasBeenPressed();
    bool hasBeenPressedAgain();
};
