#include "HardwareManager.h"

HardwareManager::HardwareManager()
    : leftButton(10, PULLUP),
    middleButton(11, PULLUP),
    rightButton(9, PULLUP),
    relay(A1, INVERTED),
    buzzer(8, NORMAL),
    clock(A3, A2),
    lightSensor(A6),
    lcd(nullptr)
{
}

uint8_t HardwareManager::getPressEvents()
{
    uint8_t pressEvents = 0;
    if (leftButton.hasBeenPressed()) {
        pressEvents |= PRESS_LEFT;
    }
    if (middleButton.hasBeenPressed()) {
        pressEvents |= PRESS_MIDDLE;
    }
    if (rightButton.hasBeenPressed()) {
        pressEvents |= PRESS_RIGHT;
    }
    return pressEvents;
}

uint16_t HardwareManager::getLightLevel()
{
    return lightSensor.getValue(100);
}

void HardwareManager::createDisplay()
{
    lcd = new LiquidCrystal(2, 3, 4, 5, 6, 7);
    lcd->begin(16, 2);
    lcd->setBacklightPin(12, POSITIVE);
}

void HardwareManager::enableDisplay()
{
    if (lcd != nullptr) {
        lcd->backlight();
        lcd->display();
    }
}

void HardwareManager::disableDisplay()
{
    if (lcd != nullptr) {
        lcd->noDisplay();
        lcd->noBacklight();
    }
}


