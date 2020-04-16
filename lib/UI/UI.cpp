#include "UI.h"

UserInterface::UserInterface(HardwareManager& hardware_, SessionManager& sessions_)
    : mainMenu(*this),
    settingSelectMenu(*this),
    clockSetupMenu(*this),
    sessionSelectMenu(*this),
    sessionSetupMenu(*this),
    currentMenu(&mainMenu),
    hardware(hardware_),
    sessions(sessions_)
{
    resetMenu();
}

void UserInterface::setMenu(Menu* menu)
{
    currentMenu = menu;
    menu->initalize(hardware.getDisplay());
}

void UserInterface::onLeftPress()
{
    currentMenu->leftPressHandler(hardware.buzzer);
}

void UserInterface::onMiddlePress()
{
    currentMenu->middlePressHandler(hardware.buzzer);
}

void UserInterface::onRightPress()
{
    currentMenu->rightPressHandler(hardware.buzzer);
}

void UserInterface::updateDisplay()
{
    currentMenu->updateDisplay(hardware.getDisplay());
}

void UserInterface::resetMenu()
{
    setMenu(&mainMenu);
}
