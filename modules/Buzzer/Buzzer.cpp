#include "Buzzer.h"

Buzzer::Buzzer(uint8_t pin_, bool inverted_) : DigitalOnePinDevice(pin_, inverted_)
{
    pinMode(pin, OUTPUT);
    stopBuzzing();
}

void Buzzer::buzz(uint8_t times, uint8_t ms)
{
#ifndef UNIT_TEST
    for (uint8_t i = 0; i < times; i++)
    {
        startBuzzing();
        delay(ms);
        stopBuzzing();
        delay(ms / 2);
    }
#endif
}
