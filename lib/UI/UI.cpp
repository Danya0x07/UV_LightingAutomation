#include <UI.h>

UserInterface::UserInterface(HardwareManager& hwm,
                             LightingSession* morningSession,
                             LightingSession* eveningSession)
    : mainMenu(*this),
    settingSelectMenu(*this),
    clockSetupMenu(*this),
    sessionSelectMenu(*this),
    sessionSetupMenu(*this),
    currentMenu(&mainMenu),
    hardwareManager(hwm),
    morningSession(morningSession),
    eveningSession(eveningSession),
    selectedSession(morningSession)
{
    resetMenu();
}

void UserInterface::setMenu(Menu* menu)
{
    currentMenu = menu;
    menu->initalize(hardwareManager.getDisplay());
}

void UserInterface::setSelectedSession(LightingSession* session)
{
    if (session != nullptr)
        selectedSession = session;
}

void UserInterface::onLeftPress()
{
    currentMenu->leftPressHandler(hardwareManager.buzzer);
}

void UserInterface::onMiddlePress()
{
    currentMenu->middlePressHandler(hardwareManager.buzzer);
}

void UserInterface::onRightPress()
{
    currentMenu->rightPressHandler(hardwareManager.buzzer);
}

void UserInterface::updateDisplay(uint8_t lightLevel)
{
    currentMenu->updateDisplay(hardwareManager.getDisplay(), lightLevel);
}

void UserInterface::resetMenu()
{
    setMenu(&mainMenu);
}
