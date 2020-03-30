#pragma once

#include <Arduino.h>
#include <RTClib.h>

class Clock
{
protected:
    using ClockType_t = RTC_DS1307;
    ClockType_t rtc;

public:
    Clock(uint8_t powerPin, uint8_t groundPin);
    DateTime getTime();
    void setTime(const DateTime&);
};