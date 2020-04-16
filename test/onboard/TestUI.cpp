#include "test.h"

void testMenuTransitions()
{
    ui.resetMenu();  // переход в главное меню
    TEST_ASSERT_TRUE(ui.getMenu() == ui.getMainMenu());

    ui.resetMenu();
    ui.onRightPress();  // переход в меню выбора настроек
    TEST_ASSERT_TRUE(ui.getMenu() == ui.getSettingSelectMenu());

    ui.resetMenu();
    ui.onRightPress();  // переход в меню выбора настроек
    ui.onRightPress();  // переход в меню выбора сеансов
    TEST_ASSERT_TRUE(ui.getMenu() == ui.getSessionSelectMenu());

    ui.resetMenu();
    ui.onRightPress();  // переход в меню выбора настроек
    ui.onRightPress();  // переход в меню выбора сеансов
    ui.onRightPress();  // переход в меню настройки сеанса
    TEST_ASSERT_TRUE(ui.getMenu() == ui.getSessionSetupMenu());

    ui.resetMenu();
    ui.onRightPress();   // переход в меню выбора настроек
    ui.onMiddlePress();  // выбор пункта CLOCK
    ui.onRightPress();   // переход в меню настройки часов
    TEST_ASSERT_TRUE(ui.getMenu() == ui.getClockSetupMenu());
}

void testMainMenuUI()
{
    hardware.relay.setState(0);
    ui.setMenu(ui.getMainMenu());

    /*
     * Тестируем поведение главного меню.
     * Тут левая кнопка ничего не делает (нужна просто для пробуждения экрана)
     * Средняя кнопка переключает состояние реле.
     * Правая кнопка осуществляет переход в меню выбора настроек.
     */
    ui.onMiddlePress();
    TEST_ASSERT_TRUE(hardware.relay.getState());
    delay(700);  // реле лучше резко не переключать
    ui.onMiddlePress();
    TEST_ASSERT_FALSE(hardware.relay.getState());
}

void testSessionConfiguringUI()
{
    sessions.morning.setActive(true);
    sessions.morning.setLightThreshold(28);
    sessions.morning.setStartTime(6, 0);
    sessions.morning.setEndTime(8, 20);

    sessions.evening.setActive(false);
    sessions.evening.setLightThreshold(31);
    sessions.evening.setStartTime(17, 40);
    sessions.evening.setEndTime(22, 21);

    /*
     * Тестируем возможность настройки сессий досветки.
     * Симулируя нажатия на кнопки, сделаем настройки вечернего сеанса
     * равными настройкам утреннего.
     */
    sessions.select(&sessions.evening);  // выбираем вечерний сеанс
    ui.setMenu(ui.getSessionSetupMenu());  // входим в меню настройки сеанса

    /* Настраиваем активность. */
    ui.onLeftPress();  // переключаем параметр active вечернего сеанса с 0 на 1
    ui.onRightPress();  // подтверждаем и переходим к следующему параметру
    TEST_ASSERT_TRUE(sessions.evening.getActive());

    /* Настраиваем порог освещённости. */
    for (uint8_t i = 0; i < 3; i++)
        ui.onMiddlePress();  // уменьшаем значение порога освещённости с 31 до 28
    ui.onRightPress();  // подтверждаем и переходим к следующему параметру
    TEST_ASSERT_EQUAL(28, sessions.evening.getLightThreshold());

    /* Настраиваем час начала. */
    for (uint8_t i = 0; i < 17 - 6; i++)
        ui.onMiddlePress();  // уменьшаем час начала с 17 до 6
    ui.onRightPress();  // подтверждаем и переходим к следующему параметру
    TEST_ASSERT_EQUAL(6, sessions.evening.getStartTime().hour());

    /* Настраиваем минуту начала. */
    for (uint8_t i = 0; i < 20; i++)
        ui.onLeftPress();  // увеличиваем минуты начала с 40 до 60, должны сброситься в 0
    ui.onRightPress();  // подтверждаем и переходим к следующему параметру
    TEST_ASSERT_EQUAL(0, sessions.evening.getStartTime().minute());

    /* Настраиваем час окончания. */
    for (uint8_t i = 0; i < 10; i++)
        ui.onLeftPress();  // увеличиваем час окончания с 22 до 8, на 24 должен сброситься в 0
    ui.onRightPress();  // подтверждаем и переходим к следующему параметру
    TEST_ASSERT_EQUAL(8, sessions.evening.getEndTime().hour());

    /* Настраиваем минуту окончания. */
    ui.onMiddlePress();  // уменьшаем минуту окончания с 21 до 20
    ui.onRightPress();  // подтверждаем и выходим в главное меню
    TEST_ASSERT_EQUAL(20, sessions.evening.getEndTime().minute());
    TEST_ASSERT_TRUE(ui.getMenu() == ui.getMainMenu());

    /* В итоге вечерний сеанс должен стать равным утреннему. */
    TEST_ASSERT_TRUE(sessions.evening == sessions.morning);
}

void testClockConfiguringUI()
{
#ifdef DEVBOARD
    /* На отладочной плате нет подключённых RTC. */
    TEST_IGNORE_MESSAGE("There is no RTC connected to devboard.");
#endif

    DateTime backup = hardware.clock.getTime();
    DateTime target(20, 5, 6, 10, 58, 0);
    DateTime source(18, 4, 7, 12, 1, 0);
    hardware.clock.setTime(source);

    /*
     * Тестируем возможность настройки даты и времени.
     * Симулируя нажатия на кнопки, установим время RTC,
     * равное target.
     */
    ui.setMenu(ui.getClockSetupMenu());  // входим в меню настройки часов

    /* Настраиваем час. */
    for (uint8_t i = 0; i < 22; i++)
        ui.onLeftPress();  // уменьшаем час с 12 до 5, используя переполнение 23 --> 0
    ui.onRightPress();  // подтверждаем и переходим к следующему параметру
    TEST_ASSERT_EQUAL(hardware.clock.getTime().hour(), target.hour());

    /* Настраиваем минуту. */
    for (uint8_t i = 0; i < 3; i++)
        ui.onMiddlePress();  // увеличиваем минуту с 1 до 58, используя переполнение 0 --> 59
    ui.onRightPress();  // подтверждаем и переходим к следующему параметру
    TEST_ASSERT_EQUAL(hardware.clock.getTime().minute(), target.minute());

    /* Настраиваем год. */
    ui.onLeftPress();  // 18 + 1 == 19
    ui.onLeftPress();  // 19 + 1 == 20
    ui.onRightPress();  // подтверждаем и переходим к следующему параметру
    TEST_ASSERT_EQUAL(hardware.clock.getTime().year(), target.year());

    /* Настраиваем месяц. */
    ui.onLeftPress();  // 4 + 1 == 5
    ui.onRightPress();  // подтверждаем и переходим к следующему параметру
    TEST_ASSERT_EQUAL(hardware.clock.getTime().month(), target.month());

    /* Настраиваем число. */
    ui.onMiddlePress();  // уменьшаем число с 7 до 6
    ui.onRightPress();  // подтверждаем и выходим в главное меню
    TEST_ASSERT_EQUAL(hardware.clock.getTime().day(), target.day());
    TEST_ASSERT_TRUE(ui.getMenu() == ui.getMainMenu());

    /* В итоге время часов должно стать равным target. */
    TEST_ASSERT_TRUE(hardware.clock.getTime() == target);

    hardware.clock.setTime(backup);
}
