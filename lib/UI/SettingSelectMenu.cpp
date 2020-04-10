#include <UI.h>

void SettingSelectMenu::initalize(LiquidCrystal* lcd)
{
    currentItem = 0;
    if (lcd) {
        lcd->clear();
        lcd->setCursor(0, 0);
        lcd->print(">SESSIONS");
        lcd->setCursor(1, 1);
        lcd->print("CLOCK");
        lcd->noBlink();
    }
}

void SettingSelectMenu::leftPressHandler(Buzzer& buzzer)
{
    buzzer.buzz(1, 150);
    currentItem++;
    if (currentItem >= NUM_OF_ITEMS)
        currentItem = NUM_OF_ITEMS - 1;
}

void SettingSelectMenu::middlePressHandler(Buzzer& buzzer)
{
    buzzer.buzz(1, 150);
    currentItem--;
    if (currentItem < 0)
        currentItem = 0;
}

void SettingSelectMenu::rightPressHandler(Buzzer& buzzer)
{
    buzzer.buzz(1, 150);
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

void SettingSelectMenu::updateDisplay(LiquidCrystal* lcd, uint8_t lightLevel)
{
    if (lcd == nullptr)
        return;
    for (uint8_t i = 0; i < NUM_OF_ITEMS; i++) {
        lcd->setCursor(0, i);
        lcd->print(currentItem == i ? '>' : ' ');
    }
}

