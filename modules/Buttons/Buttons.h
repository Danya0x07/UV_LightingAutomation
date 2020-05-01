#pragma once

#include <Arduino.h>

#include <PinDevice.h>

#define PULLUP    true
#define PULLDOWN  false

/**
 * Тактовая кнопка.
 * Инкапсулирует инициализацию пинов и определение нажатий.
 */
class Button : private DigitalOnePinDevice
{
public:
    explicit Button(uint8_t pin_, bool inverted_);

    /** Возвращает true, если кнопка зажата в данный момент, иначе false. */
    bool isPressed() { return getState(); }

    /**
     * Возвращает true, если кнопка была переведена из ненажатого состояния в нажатое.
     * После первого срабатывания. вернувшего true, возвращает false до тех пор,
     * пока кнопка не будет отпущена и нажата снова.
     * Интересный факт: если перепутать атрибут inverted, то true будет возвращаться
     * при отпускании, а не при нажатии.
     */
    bool hasBeenPressed();

    /**
     * Возвращает true, если в течение небольшого промежутка времени
     * hasBeenPressed() вернула true, иначе false.
     * Предназначена для обнаружения многократных нажатий.
     */
    bool hasBeenPressedAgain();

private:
    bool lastState;
};
