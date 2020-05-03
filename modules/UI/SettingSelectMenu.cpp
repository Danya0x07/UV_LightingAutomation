#include "UI.h"

void SettingSelectMenu::initalize(LiquidCrystal* lcd)
{
    currentItem = 0;

    if (lcd != nullptr) {
        lcd->clear();
        lcd->setCursor(1, 0);
        lcd->print(F("SESSIONS"));
        lcd->setCursor(1, 1);
        lcd->print(F("CLOCK"));
        lcd->noBlink();
    }
}

void SettingSelectMenu::update(LiquidCrystal* lcd)
{
    if (lcd == nullptr)
        return;

    for (uint8_t i = 0; i < NUM_OF_ITEMS; i++) {
        lcd->setCursor(0, i);
        lcd->print(currentItem == i ? '>' : ' ');
    }
}

void SettingSelectMenu::leftPressHandler()
{
    if (--currentItem < 0)
        currentItem = 0;
    else
        ui.makeSound(UserInterface::CHANGE_VALUE);
}

void SettingSelectMenu::middlePressHandler()
{
    if (++currentItem >= NUM_OF_ITEMS)
        currentItem = NUM_OF_ITEMS - 1;
    else
        ui.makeSound(UserInterface::CHANGE_VALUE);
}

void SettingSelectMenu::rightPressHandler()
{
    ui.makeSound(UserInterface::MENU_TRANSITION);
    switch (currentItem)
    {
    case ITEM_SESSIONS:
        ui.setMenu(ui.getSessionSelectMenu());
        break;
    case ITEM_CLOCK:
        ui.setMenu(ui.getClockSetupMenu());
        break;
    }
}
