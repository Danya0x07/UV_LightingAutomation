#include <UI.h>

const int8_t ClockSetupMenu::settingsMinBounds[NUM_OF_SETTINGS] = {
        0, 0, 1, 1, 0};
const int8_t ClockSetupMenu::settingsMaxBounds[NUM_OF_SETTINGS] = {
        23, 59, 31, 12, 99};
const uint8_t ClockSetupMenu::settingsLcdColumns[NUM_OF_SETTINGS] = {
    1, 4, 8, 11, 14};

ClockSetupMenu::ClockSetupMenu(UserInterface& ui)
    : Menu(ui), settings(), tempSettings{}, currentPos(0)
{

}

void ClockSetupMenu::initalize(LiquidCrystal* lcd)
{
    settings = ui.getClock().getTime();
    tempSettings[0] = settings.hour();
    tempSettings[1] = settings.minute();
    tempSettings[2] = settings.day();
    tempSettings[3] = settings.month();
    tempSettings[4] = settings.year();

    currentPos = 0;

    if (lcd) {
        lcd->setCursor(0, 0);
        lcd->print(F(" TIME:  DATE:   "));
        lcd->setCursor(0, 1);
        lcd->print(F("   :      /  /  "));
        lcd->blink();
    }
}

void ClockSetupMenu::leftPressHandler(Buzzer& buzzer)
{
    int8_t tempSetting = tempSettings[currentPos];
    if (++tempSetting > settingsMaxBounds[currentPos]) {
        tempSetting = settingsMinBounds[currentPos];
    }
    // TODO: Добавить коррекцию месячных рамок.

    tempSettings[currentPos] = tempSetting;
}

void ClockSetupMenu::middlePressHandler(Buzzer& buzzer)
{
    int8_t tempSetting = tempSettings[currentPos];
    if (--tempSetting < settingsMinBounds[currentPos]) {
        tempSetting = settingsMaxBounds[currentPos];
    }
    tempSettings[currentPos] = tempSetting;
}

void ClockSetupMenu::rightPressHandler(Buzzer& buzzer)
{
    settings = DateTime(tempSettings[4], tempSettings[3], tempSettings[2],
                        tempSettings[0], tempSettings[1], 0);
    ui.getClock().setTime(settings);

    if (++currentPos >= NUM_OF_SETTINGS) {
        ui.setMenu(ui.getMainMenu());
        currentPos = 0;  // на всякий случай
    }
}

void ClockSetupMenu::updateDisplay(LiquidCrystal* lcd, uint8_t lightLevel)
{
    if (lcd == nullptr)
        return;

    for (uint8_t i = 0; i < NUM_OF_SETTINGS; i++) {
        lcd->setCursor(settingsLcdColumns[i], 1);
        if (tempSettings[i] < 10)
            lcd->print('0');
        lcd->print(tempSettings[i]);
    }
    lcd->setCursor(settingsLcdColumns[currentPos], 1);
}
