#include <UI.h>

SettingSelectMenu::SettingSelectMenu(UserInterface& ui) : Menu(ui)
{

}

void SettingSelectMenu::initalize(LiquidCrystal* lcd)
{
    if (lcd) {
        lcd->noBlink();
    }
}

void SettingSelectMenu::leftPressHandler(Buzzer& buzzer)
{
    buzzer.buzz(1, 150);
}

void SettingSelectMenu::middlePressHandler(Buzzer& buzzer)
{
    buzzer.buzz(1, 150);
}

void SettingSelectMenu::rightPressHandler(Buzzer& buzzer)
{
    buzzer.buzz(1, 150);
}

void SettingSelectMenu::updateDisplay(LiquidCrystal* lcd, uint8_t lightLevel)
{
    if (lcd == nullptr)
        return;
}

