#pragma once

#include <Arduino.h>

#define NORMAL    false
#define INVERTED  true

/**
 * Базовый класс для всех периферийных устройств, для которых
 * требуется 1 пин микроконтроллера и имеет смысл резисторная подтяжка/стяжка.
 */
class DigitalOnePinDevice
{
public:
    /**
     * pin_: нормер контакта в представлении Arduino;
     * inverted_: логический уровень на пине в неактивном состоянии.
     */
    DigitalOnePinDevice(uint8_t pin_, bool inverted_)
        : pin(pin_), inverted(inverted_) {}

    /** Устанавливает логическое состояние на пине с учётом инверсности. */
    void setState(bool state) {
        digitalWrite(pin, state != inverted);
    }

    /** Возвращает логическое состояние на пине с учётом инверсности. */
    bool getState() {
        return bool(digitalRead(pin)) != inverted;
    }

protected:
    const uint8_t pin;
    const bool inverted;
};
