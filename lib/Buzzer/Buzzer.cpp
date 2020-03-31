#include "Buzzer.h"

Buzzer::Buzzer(uint8_t pin, bool inverse) : pin(pin), inverted(inverse)
{
    pinMode(pin, OUTPUT);
    stopBuzzing();
}

void Buzzer::buzz(uint8_t times, uint8_t ms)
{
    for (uint8_t i = 0; i < times; i++)
    {
        digitalWrite(pin, 1);
        delay(ms);
        digitalWrite(pin, 0);
        delay(ms / 2);
    }
}