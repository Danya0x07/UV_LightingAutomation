#include <UI.h>

UserInterface::UserInterface(Clock& clock, Relay& relay, Buzzer& buzzer,
                             LiquidCrystal* lcd,
                             LightingSession* morningSession,
                             LightingSession* eveningSession)
    : mainMenu(*this),
    settingSelectMenu(*this),
    clockSetupMenu(*this),
    sessionSelectMenu(*this),
    sessionSetupMenu(*this),
    currentMenu(&mainMenu),
    clock(clock), relay(relay), buzzer(buzzer), lcd(lcd),
    morningSession(morningSession),
    eveningSession(eveningSession),
    selectedSession(morningSession)
{
    resetMenu();
}

void UserInterface::setMenu(Menu* menu)
{
    currentMenu = menu;
    menu->initalize(lcd);   
}

void UserInterface::setSelectedSession(LightingSession* session)
{
    if (session != nullptr)
        selectedSession = session;
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
