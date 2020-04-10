#include <UI.h>

SessionSetupMenu::SessionSetupMenu(UserInterface& ui,
                                   LightingSession& morningSession,
                                   LightingSession& eveningSession)
    : Menu(ui), morningSession(morningSession), eveningSession(eveningSession)
{

}

void SessionSetupMenu::initalize(LiquidCrystal* lcd)
{

}

void SessionSetupMenu::leftPressHandler(Buzzer& buzzer)
{
    buzzer.buzz(1, 150);
}

void SessionSetupMenu::middlePressHandler(Buzzer& buzzer)
{
    buzzer.buzz(1, 150);
}

void SessionSetupMenu::rightPressHandler(Buzzer& buzzer)
{
    buzzer.buzz(2, 150);
}

void SessionSetupMenu::updateDisplay(LiquidCrystal* lcd, uint8_t lightLevel)
{
    if (lcd == nullptr)
        return;
}

