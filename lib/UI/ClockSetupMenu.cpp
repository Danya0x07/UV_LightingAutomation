#include "UI.h"

enum Month : int8_t {
    JANUARY = 1,
    FEBRUARY,
    MARCH,
    APRIL,
    MAY,
    JUNE,
    JULY,
    AUGUST,
    SEPTEMBER,
    OCTOBER,
    NOVEMBER,
    DECEMBER
};

const int8_t ClockSetupMenu::minSettingsBounds[NUM_OF_SETTINGS] = {
    0, 0, 0, 1, 1
};
const int8_t ClockSetupMenu::maxSettingsBounds[NUM_OF_SETTINGS] = {
    23, 59, 99, 12, 31
};
const uint8_t ClockSetupMenu::settingsLcdColumns[NUM_OF_SETTINGS] = {
    1, 4, 14, 11, 8
};

ClockSetupMenu::ClockSetupMenu(UserInterface& ui_)
    : Menu(ui_), tempSettings{}, currentPos(0)
{

}

void ClockSetupMenu::initalize(LiquidCrystal* lcd)
{
    DateTime settings = ui.hardware.clock.getTime();
    tempSettings[HOUR]   = settings.hour();
    tempSettings[MINUTE] = settings.minute();
    tempSettings[YEAR]   = uint8_t(settings.year() - 2000);
    tempSettings[MONTH]  = settings.month();
    tempSettings[DAY]    = settings.day();
    currentPos = 0;

    if (lcd != nullptr) {
        lcd->setCursor(0, 0);
        lcd->print(F(" TIME:  DATE:   "));
        lcd->setCursor(0, 1);
        lcd->print(F("   :      /  /  "));
        lcd->blink();
    }
}

void ClockSetupMenu::leftPressHandler()
{
   ui.makeSound(UserInterface::CHANGE_VALUE);
    int8_t setting = tempSettings[currentPos];
    if (++setting > maxSettingsBounds[currentPos]) {
        setting = minSettingsBounds[currentPos];
    }
    tempSettings[currentPos] = setting;
}

void ClockSetupMenu::middlePressHandler()
{
    ui.makeSound(UserInterface::CHANGE_VALUE);
    int8_t setting = tempSettings[currentPos];
    if (--setting < minSettingsBounds[currentPos]) {
        setting = maxSettingsBounds[currentPos];
    }
    tempSettings[currentPos] = setting;
}

void ClockSetupMenu::rightPressHandler()
{
    validateDate(&tempSettings[DAY], tempSettings[MONTH], tempSettings[YEAR]);
    DateTime settings = DateTime(tempSettings[YEAR], tempSettings[MONTH], tempSettings[DAY],
                        tempSettings[HOUR], tempSettings[MINUTE], 0);
    ui.hardware.clock.setTime(settings);
    if (++currentPos >= NUM_OF_SETTINGS) {
        ui.makeSound(UserInterface::MENU_TRANSITION);
        ui.setMenu(ui.getMainMenu());
        currentPos = 0;   // на всякий случай
    } else {
        ui.makeSound(UserInterface::CONFIRM_VALUE);
    }
}

void ClockSetupMenu::updateDisplay(LiquidCrystal* lcd)
{
    if (lcd == nullptr)
        return;

    for (uint8_t i = 0; i < NUM_OF_SETTINGS; i++) {
        lcd->setCursor(settingsLcdColumns[i], 1);
        if (tempSettings[i] < 10)
            lcd->print('0');
        lcd->print(tempSettings[i]);
    }
    lcd->setCursor(settingsLcdColumns[currentPos], 1);
}

void ClockSetupMenu::validateDate(int8_t* day, int8_t month, int16_t year)
{
    int8_t maxDay = 31;

    switch (month)
    {
    case JANUARY:
    case MARCH:
    case MAY:
    case JULY:
    case AUGUST:
    case OCTOBER:
    case DECEMBER:
        maxDay = 31;
        break;
    case APRIL:
    case JUNE:
    case SEPTEMBER:
    case NOVEMBER:
        maxDay = 30;
        break;
    case FEBRUARY:
        year += 2000;
        if ((year % 4 == 0) && (!(year % 100 == 0) || (year % 400 == 0)))
            maxDay = 29;
        else
            maxDay = 28;
    }

    if (*day > maxDay)
        *day = maxDay;
}
