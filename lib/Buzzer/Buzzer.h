#pragma once

#include <Arduino.h>

#include <PinDevice.h>

/**
 * Обёртка над пьезопищалкой для инкапсуляции инициализации пинов
 * и удобного переключения с учётом возможной инверсности управления.
 */
class Buzzer : private DigitalOnePinDevice
{
public:
    explicit Buzzer(uint8_t pin_, bool inverted_);

    void startBuzzing() { setLogicLevel(true); }
    void stopBuzzing() { setLogicLevel(false); }
    void buzz(uint8_t times, uint8_t ms);
};
