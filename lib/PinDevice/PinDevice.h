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
protected:
    const uint8_t pin;
    const bool inverted;

public:
    /**
     * pin_: нормер контакта в представлении Arduino;
     * inverted_: логический уровень на пине в неактивном состоянии.
     */
    DigitalOnePinDevice(uint8_t pin_, bool inverted_)
        : pin(pin_), inverted(inverted_) {}

    void setLogicLevel(bool logicLevel) {
        digitalWrite(pin, logicLevel != inverted);
    }

    bool getLogicLevel() {
        return bool(digitalRead(pin)) != inverted;
    }
};
