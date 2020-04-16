#pragma once

#include <Arduino.h>

#include <HardwareManager.h>
#include <SessionManager.h>

class UserInterface;

/**
 * Базовый абстрактный класс для состояний пользовательского интерфейса.
 * Является частью реализации паттерна "Состояние".
 */
class Menu
{
protected:
    UserInterface& ui;

public:
    explicit Menu(UserInterface& ui_) : ui(ui_) {}

    virtual void initalize(LiquidCrystal*) = 0;
    virtual void leftPressHandler() = 0;
    virtual void middlePressHandler() = 0;
    virtual void rightPressHandler() = 0;
    virtual void updateDisplay(LiquidCrystal*) = 0;
};

/**
 * Главное меню.
 * Является default-ным состоянием пользовательского интерфейса.
 */
class MainMenu : public Menu
{
public:
    explicit MainMenu(UserInterface& ui_) : Menu(ui_) {}

    void initalize(LiquidCrystal*) override;
    void leftPressHandler() override;
    void middlePressHandler() override;
    void rightPressHandler() override;
    void updateDisplay(LiquidCrystal*) override;
};

/** Меню выбора настроек (что собираемся настраивать). */
class SettingSelectMenu : public Menu
{
private:
    enum : int8_t {
        ITEM_SESSIONS = 0,
        ITEM_CLOCK,
        NUM_OF_ITEMS
    };
    int8_t currentItem;

public:
    explicit SettingSelectMenu(UserInterface& ui_) : Menu(ui_), currentItem(0) {}

    void initalize(LiquidCrystal*) override;
    void leftPressHandler() override;
    void middlePressHandler() override;
    void rightPressHandler() override;
    void updateDisplay(LiquidCrystal*) override;
};

/** Меню настройки часов. */
class ClockSetupMenu : public Menu
{
private:
    enum Settings : uint8_t {HOUR, MINUTE, YEAR, MONTH, DAY, NUM_OF_SETTINGS};
    static const int8_t minSettingsBounds[NUM_OF_SETTINGS];
    static const int8_t maxSettingsBounds[NUM_OF_SETTINGS];
    static const uint8_t settingsLcdColumns[NUM_OF_SETTINGS];
    int8_t tempSettings[NUM_OF_SETTINGS];
    uint8_t currentPos;

    static void validateDate(int8_t* day, int8_t month, int16_t year);

public:
    explicit ClockSetupMenu(UserInterface& ui_);

    void initalize(LiquidCrystal*) override;
    void leftPressHandler() override;
    void middlePressHandler() override;
    void rightPressHandler() override;
    void updateDisplay(LiquidCrystal*) override;
};

/** Меню выбора сеанса досветки для последующей настройки. */
class SessionSelectMenu : public Menu
{
private:
    enum : int8_t {
        ITEM_MORNING = 0,
        ITEM_EVENING,
        NUM_OF_ITEMS
    };
    int8_t currentItem;

public:
    explicit SessionSelectMenu(UserInterface& ui_) : Menu(ui_), currentItem(0) {}

    void initalize(LiquidCrystal*) override;
    void leftPressHandler() override;
    void middlePressHandler() override;
    void rightPressHandler() override;
    void updateDisplay(LiquidCrystal*) override;
};

/** Меню настройки сеанса досветки. */
class SessionSetupMenu : public Menu
{
private:
    enum Settings : int8_t {
        ACTIVITY, THRESHOLD,
        STARTHOUR, STARTMINUTE,
        ENDHOUR, ENDMINUTE,
        NUM_OF_SETTINGS
    };
    static const int8_t maxSettingsBounds[NUM_OF_SETTINGS];
    static const uint8_t settingsLcdRows[NUM_OF_SETTINGS];
    static const uint8_t settingsLcdColumns[NUM_OF_SETTINGS];
    uint8_t tempSettings[NUM_OF_SETTINGS];
    uint8_t currentPos;

public:
    explicit SessionSetupMenu(UserInterface& ui_);

    void initalize(LiquidCrystal*) override;
    void leftPressHandler() override;
    void middlePressHandler() override;
    void rightPressHandler() override;
    void updateDisplay(LiquidCrystal*) override;
};

/**
 * Представление пользовательского интерфейса в виде конечного автомата,
 * в котором состояниями являются различные меню.
 * Предоставляет возможность меню взаимодействовать с компонентами системы.
 */
class UserInterface
{
private:
    MainMenu mainMenu;
    SettingSelectMenu settingSelectMenu;
    ClockSetupMenu clockSetupMenu;
    SessionSelectMenu sessionSelectMenu;
    SessionSetupMenu sessionSetupMenu;
    Menu* currentMenu;

public:
    enum Sound : uint8_t {
        DISPLAY_AWAKE,
        CHANGE_VALUE,
        CONFIRM_VALUE,
        MENU_TRANSITION
    };

    HardwareManager& hardware;
    SessionManager& sessions;

    explicit UserInterface(HardwareManager& hardware_, SessionManager& sessions_);

    Menu* getMainMenu() { return &mainMenu; }
    Menu* getSettingSelectMenu() { return &settingSelectMenu; }
    Menu* getClockSetupMenu() { return &clockSetupMenu; }
    Menu* getSessionSelectMenu() { return &sessionSelectMenu; }
    Menu* getSessionSetupMenu() { return &sessionSetupMenu; }
#ifdef UNIT_TEST
    Menu* getMenu() { return currentMenu; }
#endif
    void setMenu(Menu*);

    void makeSound(Sound);

    void onLeftPress();
    void onMiddlePress();
    void onRightPress();
    void updateDisplay();
    void resetMenu();
};
