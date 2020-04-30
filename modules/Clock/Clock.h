#pragma once

#include <Arduino.h>
#include <RTClib.h>

/**
 * Обёртка над RTC для инкапсуляции инициализации пинов
 * (VCC и GND подключены к GPIO МК, а не к соответствующим шинам)
 * и, в перспективе, удобной настройки текущего времени.
 */
class Clock
{
private:
    RTC_DS1307 rtc;

public:
    explicit Clock(uint8_t powerPin, uint8_t groundPin);
    DateTime getTime();
    void setTime(const DateTime&);
};