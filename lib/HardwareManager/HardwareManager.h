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
    enum PressEvent : uint8_t {
        PRESS_LEFT   = 1 << 0,
        PRESS_MIDDLE = 1 << 1,
        PRESS_RIGHT  = 1 << 2
    };

    Button leftButton;
    Button middleButton;
    Button rightButton;
    Relay relay;
    Buzzer buzzer;
    Clock clock;
    LightSensor lightSensor;

    HardwareManager();
    HardwareManager(const HardwareManager& copy) = delete;
    HardwareManager& operator=(const HardwareManager& copy) = delete;

    /**
     * Возвращает битовую маску из перечисление PressEvent, содержащую
     * произошедшие в момент вызова нажатия.
     */
    uint8_t getPressEvents();

    /** Возвращает текущий уровень освещённости в диапазоне (0 - 100). */
    uint16_t getLightLevel();

    void enableDisplay();
    void disableDisplay();
    LiquidCrystal* getDisplay() { return lcd; }
};
