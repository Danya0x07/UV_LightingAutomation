#include <UI.h>

UserInterface::UserInterface(Clock& clock, Relay& relay, Buzzer& buzzer,
                             LiquidCrystal* lcd,
                             LightingSession& morningSession,
                             LightingSession& eveningSession)
    : mainMenu(*this),
    settingsSelectMenu(*this),
    clockSetupMenu(*this),
    sessionSelectMenu(*this),
    sessionSetupMenu(*this, morningSession, eveningSession),
    clock(clock), relay(relay), buzzer(buzzer), lcd(lcd)
{

}

void UserInterface::setMenu(Menu* menu)
{

}

void UserInterface::onLeftPress()
{

}

void UserInterface::onMiddlePress()
{

}

void UserInterface::onRightPress()
{

}

void UserInterface::updateDisplay(uint8_t lightLevel)
{

}

void UserInterface::resetMenu()
{

}
