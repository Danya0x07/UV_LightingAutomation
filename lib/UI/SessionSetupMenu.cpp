#include "UI.h"

const int8_t SessionSetupMenu::maxSettingsBounds[NUM_OF_SETTINGS] = {
    1, 100, 23, 59, 23, 59
};
const uint8_t SessionSetupMenu::settingsLcdRows[NUM_OF_SETTINGS] = {
    0, 0, 1, 1, 1, 1
};
const uint8_t SessionSetupMenu::settingsLcdColumns[NUM_OF_SETTINGS] = {
    4, 12, 2, 5, 10, 13
};

SessionSetupMenu::SessionSetupMenu(UserInterface& ui_)
    : Menu(ui_), tempSettings{}, currentPos(0)
{

}

void SessionSetupMenu::initalize(LiquidCrystal* lcd)
{
    LightingSession* selectedSession = ui.sessions.getSelected();
    tempSettings[ACTIVITY] = selectedSession->getActive();
    tempSettings[THRESHOLD] = selectedSession->getLightThreshold();
    selectedSession->getStartTime(&tempSettings[STARTHOUR], &tempSettings[STARTMINUTE]);
    selectedSession->getEndTime(&tempSettings[ENDHOUR], &tempSettings[ENDMINUTE]);
    currentPos = 0;

    if (lcd != nullptr) {
        lcd->setCursor(0, 0);
        lcd->print(F("  A:      L:   %"));
        lcd->setCursor(0, 1);
        lcd->print(F("S:  :   E:  :   "));
        lcd->blink();
    }
}

void SessionSetupMenu::leftPressHandler(Buzzer& buzzer)
{
    buzzer.buzz(1, 150);
    int8_t setting = tempSettings[currentPos];
    if (++setting > maxSettingsBounds[currentPos]) {
        setting = 0;
    }
    tempSettings[currentPos] = setting;
}

void SessionSetupMenu::middlePressHandler(Buzzer& buzzer)
{
    buzzer.buzz(1, 150);
    int8_t setting = tempSettings[currentPos];
    if (--setting < 0) {
        setting = maxSettingsBounds[currentPos];
    }
    tempSettings[currentPos] = setting;
}

void SessionSetupMenu::rightPressHandler(Buzzer& buzzer)
{
    buzzer.buzz(2, 150);
    LightingSession* selectedSession = ui.sessions.getSelected();
    selectedSession->setActive(bool(tempSettings[ACTIVITY]));
    selectedSession->setLightThreshold(tempSettings[THRESHOLD]);
    selectedSession->setStartTime(tempSettings[STARTHOUR], tempSettings[STARTMINUTE]);
    selectedSession->setEndTime(tempSettings[ENDHOUR], tempSettings[ENDMINUTE]);

    if (++currentPos >= NUM_OF_SETTINGS) {
        ui.sessions.save();
        ui.setMenu(ui.getMainMenu());
    }
}

void SessionSetupMenu::updateDisplay(LiquidCrystal* lcd)
{
    if (lcd == nullptr)
        return;

    for (uint8_t i = 0; i < NUM_OF_SETTINGS; i++) {
        lcd->setCursor(settingsLcdColumns[i], settingsLcdRows[i]);
        if (i == THRESHOLD) {
            lcd->print(tempSettings[i]);
            if (tempSettings[i] < 100)
                lcd->print(' ');
            if (tempSettings[i] < 10)
                lcd->print(' ');
        } else {
            if (tempSettings[i] < 10 && i != ACTIVITY)
                lcd->print('0');
            lcd->print(tempSettings[i]);
        }
    }
    lcd->setCursor(settingsLcdColumns[currentPos], settingsLcdRows[currentPos]);
}

