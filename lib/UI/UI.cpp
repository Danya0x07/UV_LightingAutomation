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
    resetMenu();
}

void UserInterface::setMenu(Menu* menu)
{
    menu->initalize(lcd);
    currentMenu = menu;
}

void UserInterface::onLeftPress()
{
    currentMenu->leftPressHandler(buzzer);
}

void UserInterface::onMiddlePress()
{
    currentMenu->middlePressHandler(buzzer);
}

void UserInterface::onRightPress()
{
    currentMenu->rightPressHandler(buzzer);
}

void UserInterface::updateDisplay(uint8_t lightLevel)
{
    currentMenu->updateDisplay(lcd, lightLevel);
}

void UserInterface::resetMenu()
{
    setMenu(&mainMenu);
}
