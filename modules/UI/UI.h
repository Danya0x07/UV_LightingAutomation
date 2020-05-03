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
public:
    explicit Menu(UserInterface& ui_) : ui(ui_) {}

    /* Инициализация и обновление дисплея поддерживаются любым меню. */
    virtual void initalize(LiquidCrystal*) = 0;
    virtual void update(LiquidCrystal*) = 0;

    /* Обработчики однократных нажатий поддерживаются любым меню. */
    virtual void leftPressHandler() = 0;
    virtual void middlePressHandler() = 0;
    virtual void rightPressHandler() = 0;

    /* Некоторые меню могут поддерживать обаботчик удержания. */
    virtual void pressHoldHandler() {}

protected:
    UserInterface& ui;
};

/**
 * Главное меню.
 * Является default-ным состоянием пользовательского интерфейса.
 * Имеет вид:
 *  ________________    D - Дата: ЧЧ:ММ ДД/ММ/ГГ
 * |D:15:30 05/04/20|   L - уровень освещённости(0-100)
 * |L:48%  R:OFF  ME|   R - состояние реле(ON/OFF)
 *  ----------------    ME - флаги прошедших сеансов(MORNING, EVENING)
 */
class MainMenu : public Menu
{
public:
    explicit MainMenu(UserInterface& ui_) : Menu(ui_) {}

    void initalize(LiquidCrystal*) override;
    void update(LiquidCrystal*) override;

    void leftPressHandler() override;
    void middlePressHandler() override;
    void rightPressHandler() override;
};

/**
 * Меню выбора настроек (что собираемся настраивать).
 * Имеет вид:
 *  ________________    > - метка выбора
 * |>SESSIONS       |
 * | CLOCK          |
 *  ----------------
 */
class SettingSelectMenu : public Menu
{
public:
    explicit SettingSelectMenu(UserInterface& ui_) : Menu(ui_), currentItem(0) {}

    void initalize(LiquidCrystal*) override;
    void update(LiquidCrystal*) override;

    void leftPressHandler() override;
    void middlePressHandler() override;
    void rightPressHandler() override;

private:
    enum : int8_t {
        ITEM_SESSIONS = 0,
        ITEM_CLOCK,
        NUM_OF_ITEMS
    };
    int8_t currentItem;
};

/**
 * Меню настройки часов.
 * Имеет вид:
 *  ________________
 * | TIME:  DATE:   |
 * | 18:43  17/04/20|
 *  ----------------
 */
class ClockSetupMenu : public Menu
{
public:
    explicit ClockSetupMenu(UserInterface& ui_);

    void initalize(LiquidCrystal*) override;
    void update(LiquidCrystal*) override;

    void leftPressHandler() override;
    void middlePressHandler() override;
    void rightPressHandler() override;

    void pressHoldHandler() override;

private:
    enum Settings : uint8_t {HOUR, MINUTE, YEAR, MONTH, DAY, NUM_OF_SETTINGS};
    static const int8_t minSettingsBounds[NUM_OF_SETTINGS];
    static const int8_t maxSettingsBounds[NUM_OF_SETTINGS];
    static const uint8_t settingsLcdColumns[NUM_OF_SETTINGS];
    int8_t tempSettings[NUM_OF_SETTINGS];
    uint8_t currentPos;

    void (ClockSetupMenu::*lastHandler)();

    static void validateDate(int8_t* day, int8_t month, int16_t year);
};

/**
 * Меню выбора сеанса досветки для последующей настройки.
 * Имеет вид:
 *  ________________
 * |>MORNING        |
 * | EVENING        |
 *  ----------------
 */
class SessionSelectMenu : public Menu
{
public:
    explicit SessionSelectMenu(UserInterface& ui_) : Menu(ui_), currentItem(0) {}

    void initalize(LiquidCrystal*) override;
    void update(LiquidCrystal*) override;

    void leftPressHandler() override;
    void middlePressHandler() override;
    void rightPressHandler() override;

private:
    enum : int8_t {
        ITEM_MORNING = 0,
        ITEM_EVENING,
        NUM_OF_ITEMS
    };
    int8_t currentItem;
};

/**
 * Меню настройки сеанса досветки.
 * Имеет вид:
 *  ________________    A - активность сеанса(1/0)
 * |  A:1     L:100%|   L - порог освещённости(0-100)
 * |S:06:30 E:10:00 |   S - время начала
 *  ----------------    E - время окончания
 */
class SessionSetupMenu : public Menu
{
public:
    explicit SessionSetupMenu(UserInterface& ui_);

    void initalize(LiquidCrystal*) override;
    void update(LiquidCrystal*) override;

    void leftPressHandler() override;
    void middlePressHandler() override;
    void rightPressHandler() override;

    void pressHoldHandler() override;

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

    void (SessionSetupMenu::*lastHandler)();
};

/**
 * Представление пользовательского интерфейса в виде конечного автомата,
 * в котором состояниями являются различные меню.
 * Предоставляет возможность меню взаимодействовать с компонентами системы.
 */
class UserInterface
{
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
    Menu* getMenu() { return currentMenu; }
    void setMenu(Menu*);
    void resetMenu();

    void onLeftPress();
    void onMiddlePress();
    void onRightPress();
    void onPressHold();

    /* Несущественны для юнит-тестов. */
    void makeSound(Sound);
    void updateDisplay();

private:
    MainMenu mainMenu;
    SettingSelectMenu settingSelectMenu;
    ClockSetupMenu clockSetupMenu;
    SessionSelectMenu sessionSelectMenu;
    SessionSetupMenu sessionSetupMenu;
    Menu* currentMenu;
};
