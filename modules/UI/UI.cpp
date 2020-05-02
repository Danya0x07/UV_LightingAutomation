#include "UI.h"

UserInterface::UserInterface(HardwareManager& hardware_, SessionManager& sessions_)
    : hardware(hardware_),
    sessions(sessions_),
    mainMenu(*this),
    settingSelectMenu(*this),
    clockSetupMenu(*this),
    sessionSelectMenu(*this),
    sessionSetupMenu(*this),
    currentMenu(&mainMenu)
{
}

void UserInterface::setMenu(Menu* menu)
{
    menu->initalize(hardware.getDisplay());
    currentMenu = menu;
}

void UserInterface::makeSound(Sound sound)
{
    switch (sound)
    {
    case DISPLAY_AWAKE:   hardware.buzzer.buzz(1, 150); break;
    case CHANGE_VALUE:    hardware.buzzer.buzz(1, 80);  break;
    case CONFIRM_VALUE:   hardware.buzzer.buzz(1, 200); break;
    case MENU_TRANSITION: hardware.buzzer.buzz(2, 100); break;
    }
}

void UserInterface::onLeftPress()
{
    currentMenu->leftPressHandler();
}

void UserInterface::onMiddlePress()
{
    currentMenu->middlePressHandler();
}

void UserInterface::onRightPress()
{
    currentMenu->rightPressHandler();
}

void UserInterface::onPressRepeat()
{
    currentMenu->pressRepeatHandler();
}

void UserInterface::updateDisplay()
{
    currentMenu->updateDisplay(hardware.getDisplay());
}

void UserInterface::resetMenu()
{
    setMenu(&mainMenu);
}
