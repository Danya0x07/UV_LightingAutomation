#pragma once

#include <Arduino.h>
#include <LiquidCrystal.h>

#include <Clock.h>
#include <Relay.h>
#include <Buzzer.h>
#include <LightingSession.h>

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
    explicit Menu(UserInterface& ui) : ui(ui) {}

    virtual void initalize(LiquidCrystal*) = 0;
    virtual void leftPressHandler(Buzzer&) = 0;
    virtual void middlePressHandler(Buzzer&) = 0;
    virtual void rightPressHandler(Buzzer&) = 0;
    virtual void updateDisplay(LiquidCrystal*, uint8_t lightLevel) = 0;
};

/**
 * Главное меню.
 * Является default-ным состоянием пользовательского интерфейса.
 */
class MainMenu : public Menu
{
public:
    explicit MainMenu(UserInterface&) : Menu(ui) {}

    void initalize(LiquidCrystal*) override;
    void leftPressHandler(Buzzer&) override;
    void middlePressHandler(Buzzer&) override;
    void rightPressHandler(Buzzer&) override;
    void updateDisplay(LiquidCrystal*, uint8_t lightLevel) override;
};

/** Меню выбора настроек (что собираемся настраивать). */
class SettingSelectMenu : public Menu
{
private:
    enum : int8_t {
        ITEM_SESSIONS,
        ITEM_CLOCK,
        NUM_OF_ITEMS
    };
    int8_t currentItem = ITEM_SESSIONS;

public:
    explicit SettingSelectMenu(UserInterface&) : Menu(ui) {}

    void initalize(LiquidCrystal*) override;
    void leftPressHandler(Buzzer&) override;
    void middlePressHandler(Buzzer&) override;
    void rightPressHandler(Buzzer&) override;
    void updateDisplay(LiquidCrystal*, uint8_t lightLevel) override;
};

/** Меню настройки часов. */
class ClockSetupMenu : public Menu
{
private:
    static const uint8_t NUM_OF_TEMP_SETTINGS = 5;
    uint8_t tempSettings[NUM_OF_TEMP_SETTINGS];
    uint8_t currentPos = 0;

public:
    explicit ClockSetupMenu(UserInterface&);

    void initalize(LiquidCrystal*) override;
    void leftPressHandler(Buzzer&) override;
    void middlePressHandler(Buzzer&) override;
    void rightPressHandler(Buzzer&) override;
    void updateDisplay(LiquidCrystal*, uint8_t lightLevel) override;
};

/** Меню выбора сеанса досветки для последующей настройки. */
class SessionSelectMenu : public Menu
{
private:
    enum : int8_t {
        ITEM_MORNING,
        ITEM_EVENING,
        NUM_OF_ITEMS
    } currentItem = ITEM_MORNING;

public:
    explicit SessionSelectMenu(UserInterface&);

    void initalize(LiquidCrystal*) override;
    void leftPressHandler(Buzzer&) override;
    void middlePressHandler(Buzzer&) override;
    void rightPressHandler(Buzzer&) override;
    void updateDisplay(LiquidCrystal*, uint8_t lightLevel) override;
};

/** Меню настройки сеанса досветки. */
class SessionSetupMenu : public Menu
{
private:
    static const uint8_t NUM_OF_TEMP_SETTINGS = 6;
    uint8_t tempSettings[NUM_OF_TEMP_SETTINGS];
    uint8_t currentPos = 0;
    LightingSession& morningSession;
    LightingSession& eveningSession;

public:
    explicit SessionSetupMenu(UserInterface&, LightingSession&, LightingSession&);

    void initalize(LiquidCrystal*) override;
    void leftPressHandler(Buzzer&) override;
    void middlePressHandler(Buzzer&) override;
    void rightPressHandler(Buzzer&) override;
    void updateDisplay(LiquidCrystal*, uint8_t lightLevel) override;
};

/**
 * Представление пользовательского интерфейса в виде конечного автомата,
 * в котором состояниями являются различные меню.
 * Одновременно является посредником меню для доступа к компонентам системы.
 */
class UserInterface
{
private:
    MainMenu mainMenu;
    SettingSelectMenu settingsSelectMenu;
    ClockSetupMenu clockSetupMenu;
    SessionSelectMenu sessionSelectMenu;
    SessionSetupMenu sessionSetupMenu;
    Menu* currentMenu;

    Clock& clock;
    Relay& relay;
    Buzzer& buzzer;
    LiquidCrystal* const lcd;

public:
    explicit UserInterface(Clock&, Relay&, Buzzer&, LiquidCrystal*,
                           LightingSession&, LightingSession&);

    Menu* getMainMenu() { return &mainMenu; }
    Menu* getSettingsSelectMenu() { return &settingsSelectMenu; }
    Menu* getClockSetupMenu() { return &clockSetupMenu; }
    Menu* getSessionSelectMenu() { return &sessionSelectMenu; }
    Menu* getSessionSetupMenu() { return &sessionSetupMenu; }
    void setMenu(Menu*);

    Clock& getClock() { return clock; }
    Relay& getRelay() { return relay; }

    void onLeftPress();
    void onMiddlePress();
    void onRightPress();
    void updateDisplay(uint8_t lightLevel);
    void resetMenu();
};
