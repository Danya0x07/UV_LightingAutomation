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

    /** Перечисление событий кнопок. */
    enum ButtonEvent : uint8_t {
        PRESS_LEFT   = 1 << 0,
        PRESS_MIDDLE = 1 << 1,
        PRESS_RIGHT  = 1 << 2,
    };

    /** Перечисление состояний кнопок. */
    enum ButtonStatus : uint8_t {
        PRESSED_LEFT   = 1 << 0,
        PRESSED_MIDDLE = 1 << 1,
        PRESSED_RIGHT  = 1 << 2
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
     * Возвращает битовую маску из перечисления ButtonEvent,
     * содержащую события кнопок, произошедшие со времени последнего вызова.
     */
    uint8_t getButtonsEvents();

    /**
     * Возвращает битовую маску из перечисления ButtonStatus,
     * содержащую текущее состояние кнопок.
     */
    uint8_t getButtonsStatus();

    /** Возвращает текущий уровень освещённости в диапазоне (0 - 100). */
    uint16_t getLightLevel();

    /**
     * Создаёт и инициализирует дисплей.
     * Должна вызываться после инициализации фреймворка Arduino (т.е. в setup).
     */
    void createDisplay();

    /** Включает подсветку дисплея (если он есть) и отображение на нём данных. */
    void enableDisplay();

    /** Выключает подсветку дисплея (если он есть) и отображение на нём данных. */
    void disableDisplay();

    /**
     * Возвращает адрес дисплея, если таковой был создан
     * вызовом HardwareManager::createDisplay(), иначе nullptr.
     */
    LiquidCrystal* getDisplay() { return lcd; }

private:
    LiquidCrystal* lcd;
};
