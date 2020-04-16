#include "UI.h"

void MainMenu::initalize(LiquidCrystal* lcd)
{
    if (lcd != nullptr) {
        lcd->setCursor(0, 0);
        lcd->print(F("D:  :     /  /  "));
        lcd->setCursor(0, 1);
        lcd->print(F("L:     R:       "));
        lcd->noBlink();
    }
}

void MainMenu::leftPressHandler(Buzzer& buzzer)
{
    buzzer.buzz(1, 170);
}

void MainMenu::middlePressHandler(Buzzer& buzzer)
{
    buzzer.buzz(2, 150);
    ui.hardware.relay.switchState();
}

void MainMenu::rightPressHandler(Buzzer& buzzer)
{
    buzzer.buzz(1, 150);
    ui.setMenu(ui.getSettingSelectMenu());
}

void MainMenu::updateDisplay(LiquidCrystal* lcd)
{
    DateTime dateTime = ui.hardware.clock.getTime();
    uint8_t hour = dateTime.hour();
    uint8_t minute = dateTime.minute();
    uint8_t day = dateTime.day();
    uint8_t month = dateTime.month();
    uint8_t year = (dateTime.year() - 2000) % 100;
    uint8_t lightLevel = ui.hardware.getLightLevel();
    bool relayState = ui.hardware.relay.getState();
    uint8_t performedSessions = ui.sessions.getPerformedToday();

    if (lcd == nullptr)
        return;

    /* Печатаем время. */
    lcd->setCursor(2, 0);
    if (hour < 10)
        lcd->print('0');
    lcd->print(hour);

    lcd->setCursor(5, 0);
    if (minute < 10)
        lcd->print('0');
    lcd->print(minute);

    /* Печатаем дату. */
    lcd->setCursor(8, 0);
    if (day < 10)
        lcd->print('0');
    lcd->print(day);

    lcd->setCursor(11, 0);
    if (month < 10)
        lcd->print('0');
    lcd->print(month);

    lcd->setCursor(14, 0);
    if (year < 10)
        lcd->print('0');
    lcd->print(year);

    /* Печатаем уровень освещённости. */
    lcd->setCursor(2, 1);
    lcd->print(lightLevel);
    lcd->print('%');
    if (lightLevel < 100)
        lcd->print(' ');
    if (lightLevel < 10)
        lcd->print(' ');

    /* Печатаем состояние реле. */
    lcd->setCursor(9, 1);
    lcd->print(relayState ? F("ON ") : F("OFF"));

    /* Печатаем прошедшие сеансы. */
    lcd->setCursor(14, 1);
    lcd->print(performedSessions & SessionManager::SESSION_MORNING ? 'M' : ' ');
    lcd->print(performedSessions & SessionManager::SESSION_EVENING ? 'E' : ' ');
}
