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

}

void SessionSetupMenu::middlePressHandler(Buzzer& buzzer)
{

}

void SessionSetupMenu::rightPressHandler(Buzzer& buzzer)
{

}

void SessionSetupMenu::updateDisplay(LiquidCrystal* lcd, uint8_t lightLevel)
{

}

