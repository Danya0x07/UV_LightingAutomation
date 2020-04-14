#include "Clock.h"

Clock::Clock(uint8_t powerPin, uint8_t groundPin)
{
    pinMode(powerPin, OUTPUT);
    pinMode(groundPin, OUTPUT);
    digitalWrite(groundPin, 0);
    digitalWrite(powerPin, 1);
    rtc.begin();
}

DateTime Clock::getTime()
{
    return rtc.now();
}

void Clock::setTime(const DateTime& dt)
{
    rtc.adjust(dt);
}