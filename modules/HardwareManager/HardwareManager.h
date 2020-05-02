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
public:
    enum ButtonEvent : uint8_t {
        PRESS_LEFT   = 1 << 0,
        PRESS_MIDDLE = 1 << 1,
        PRESS_RIGHT  = 1 << 2,
        HOLD_LEFT   = 1 << 3,
        HOLD_MIDDLE = 1 << 4,
        HOLD_RIGHT  = 1 << 5
    };
    static const uint8_t PRESS_OCCURED_MASK = 0x07;

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
     * Возвращает битовую маску из перечисление ButtonEvent,
     * содержащую текущее состояние кнопок.
     */
    uint8_t getButtonsEvents();

    /** Возвращает текущий уровень освещённости в диапазоне (0 - 100). */
    uint16_t getLightLevel();

    /**
     * Создаёт и инициализирует дисплей.
     * Должна вызываться после инициализации фреймворка Arduino (т.е. в setup).
     */
    void createDisplay();

    /** Включает подсветку дисплея и отображение на нём данных. */
    void enableDisplay();

    /** Выключает подсветку дисплея и отображение на нём данных. */
    void disableDisplay();

    /**
     * Возвращает адрес дисплея, если таковой был создан
     * вызовом HardwareManager::createDisplay(), иначе nullptr.
     */
    LiquidCrystal* getDisplay() { return lcd; }

private:
    LiquidCrystal* lcd;
};
