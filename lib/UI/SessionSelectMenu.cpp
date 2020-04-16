#include "UI.h"

void SessionSelectMenu::initalize(LiquidCrystal* lcd)
{
    currentItem = 0;

    if (lcd != nullptr) {
        lcd->clear();
        lcd->setCursor(1, 0);
        lcd->print(F("MORNING"));
        lcd->setCursor(1, 1);
        lcd->print(F("EVENING"));
        lcd->noBlink();
    }
}

void SessionSelectMenu::leftPressHandler(Buzzer& buzzer)
{
    buzzer.buzz(1, 150);
    if (--currentItem < 0)
        currentItem = 0;
}

void SessionSelectMenu::middlePressHandler(Buzzer& buzzer)
{
    buzzer.buzz(1, 150);
    if (++currentItem >= NUM_OF_ITEMS)
        currentItem = NUM_OF_ITEMS - 1;
}

void SessionSelectMenu::rightPressHandler(Buzzer& buzzer)
{
    buzzer.buzz(2, 150);
    switch (currentItem)
    {
    case ITEM_MORNING:
        ui.sessions.select(&ui.sessions.morning);
        break;
    case ITEM_EVENING:
        ui.sessions.select(&ui.sessions.evening);
        break;
    }
    ui.setMenu(ui.getSessionSetupMenu());
}

void SessionSelectMenu::updateDisplay(LiquidCrystal* lcd)
{
    if (lcd == nullptr)
        return;

    for (uint8_t i = 0; i < NUM_OF_ITEMS; i++) {
        lcd->setCursor(0, i);
        lcd->print(currentItem == i ? '>' : ' ');
    }
}

