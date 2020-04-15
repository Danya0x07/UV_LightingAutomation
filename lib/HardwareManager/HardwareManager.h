#pragma once

#include <Arduino.h>
#include <LiquidCrystal.h>

#include <Buttons.h>
#include <Relay.h>
#include <Buzzer.h>
#include <Clock.h>
#include <LightSensor.h>

/** Фасад для аппаратного обеспечения системы. */
class HardwareManager
{
private:
    LiquidCrystal* lcd;

public:
    Button leftButton;
    Button middleButton;
    Button rightButton;
    Relay relay;
    Buzzer buzzer;
    Clock clock;
    LightSensor lightSensor;

    enum PressEvent : uint8_t {
        PRESS_LEFT   = 1 << 0,
        PRESS_MIDDLE = 1 << 1,
        PRESS_RIGHT  = 1 << 2,
        PRESS_WAS = PRESS_LEFT | PRESS_MIDDLE | PRESS_RIGHT
    };

    HardwareManager();
    HardwareManager(const HardwareManager& copy) = delete;
    HardwareManager& operator=(const HardwareManager& copy) = delete;

    uint8_t getPressEvents();
    uint16_t getLightLevel();

    void enableDisplay();
    void disableDisplay();
    LiquidCrystal* getDisplay() { return lcd; }
};
