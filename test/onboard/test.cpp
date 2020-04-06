#include <Arduino.h>
#include <unity.h>

#include <Buttons.h>
#include <Relay.h>
#include <Buzzer.h>
#include <Clock.h>
#include <LightingSession.h>
#include <UI.h>

Relay relay(A1, true);
Clock rtclock(A3, A2);
Buzzer buzzer(8, false);
LightingSession morningSession, eveningSession;
UserInterface ui(rtclock, relay, buzzer, nullptr, morningSession, eveningSession);

void testRelay()
{
    relay.setState(0);
    TEST_ASSERT_EQUAL(relay.getState(), false);
    relay.setState(1);
    TEST_ASSERT_EQUAL(relay.getState(), true);
    delay(700);  // реле лучше резко не переключать
    relay.switchState();
    TEST_ASSERT_EQUAL(relay.getState(), false);
}

void testButtons()
{
    Button leftButton(10, true);
    Button middleButton(11, true);
    Button rightButton(9, true);

    /*
     * В состоянии покоя кнопки не должны быть нажаты, 
     * если это не так, значит внутренние подтягивающие 
     * резисторы вышли из строя.
     */
    TEST_ASSERT_FALSE(leftButton.isPressed());
    TEST_ASSERT_FALSE(middleButton.isPressed());
    TEST_ASSERT_FALSE(rightButton.isPressed());
}

void testClock()
{
#ifdef DEVBOARD
    /* На отладочной плате нет подключённых RTC. */
    TEST_IGNORE_MESSAGE("There is no RTC connected to devboard.");
#endif
    DateTime dt = rtclock.getTime();
    delay(1001);
    /* Проверяем, что часы идут. */
    TEST_ASSERT_TRUE(rtclock.getTime() > dt);
}

void testLightingSession()
{
    LightingSession lightingSession;
    lightingSession.setActive(true);
    lightingSession.setLightThreshold(33);
    lightingSession.setStartTime(6, 30);
    lightingSession.setEndTime(9, 0);

    /* Проверяем рамки яркости. */
    TEST_ASSERT_TRUE(lightingSession.hasToBeUnderway(DateTime(0, 0, 0, 7, 59, 0), 33));
    TEST_ASSERT_FALSE(lightingSession.hasToBeUnderway(DateTime(0, 0, 0, 7, 59, 0), 34));

    /* Проверяем активность/неактивность. */
    lightingSession.setActive(false);
    TEST_ASSERT_FALSE(lightingSession.hasToBeUnderway(DateTime(0, 0, 0, 7, 59, 0), 20));
    lightingSession.setActive(true);

    /* Проверяем временные рамки. */
    TEST_ASSERT_FALSE(lightingSession.hasToBeUnderway(DateTime(0, 0, 0, 6, 29, 0), 20));
    TEST_ASSERT_FALSE(lightingSession.hasToBeUnderway(DateTime(0, 0, 0, 9, 1, 0), 20));
    
    /*
     * Проверяем корректность сохранения данных о сеансе в EEPROM.
     * для этого сохраняем сеанс по определённому адресу, а потом
     * ещё раз, впритык к уже сохранённым данным. Далее сравниваем
     * загруженный по первоначальному адресу сеанс: если они не равны,
     * значит второе сохранение повредило данные первого.
     */
    uint16_t address = 100;
    lightingSession.saveToEeprom(address);
    lightingSession.saveToEeprom(address + LightingSession::getActualEepromPayloadSize());
    LightingSession otherLightingSession;
    otherLightingSession.loadFromEeprom(address);
    TEST_ASSERT_TRUE(otherLightingSession == lightingSession);
}

void testMainMenuUI()
{
    relay.setState(0);
    ui.resetMenu();

    /* 
     * Тестируем поведение главного меню. 
     * Тут левая кнопка ничего не делает (нужна просто для пробуждения экрана)
     * Средняя кнопка переключает состояние реле.
     * Правая кнопка осуществляет переход в меню выбора настроек.
     */
    ui.onMiddlePress();
    TEST_ASSERT_TRUE(relay.getState());
    delay(700);  // реле лучше резко не переключать
    ui.onMiddlePress();
    TEST_ASSERT_FALSE(relay.getState());
}

void testSessionConfiguringUI()
{
    morningSession.setActive(true);
    morningSession.setLightThreshold(28);
    morningSession.setStartTime(6, 0);
    morningSession.setEndTime(8, 20);

    eveningSession.setActive(false);
    eveningSession.setLightThreshold(31);
    eveningSession.setStartTime(17, 40);
    eveningSession.setEndTime(22, 21);

    /*
     * Тестируем возможность настройки сессий досветки.
     * Симулируя нажатия на кнопки, сделаем настройки вечернего сеанса
     * равными настройкам утреннего.
     */
    ui.onRightPress();  // входим в меню выбора настроек
    ui.onRightPress();  // входим в меню выбора сеансов настройки
    ui.onMiddlePress();  // выбираем вечерний сеанс
    ui.onRightPress();  // входим в меню настройки сеанса

    /* Настраиваем активность. */
    ui.onLeftPress();  // переключаем параметр active вечернего сеанса с 0 на 1
    ui.onRightPress();  // подтверждаем и переходим к следующему параметру
    TEST_ASSERT_TRUE(eveningSession.getActive());

    /* Настраиваем порог освещённости. */
    for (uint8_t i = 0; i < 3; i++)
        ui.onMiddlePress();  // уменьшаем значение порога освещённости с 31 до 28
    ui.onRightPress();  // подтверждаем и переходим к следующему параметру
    TEST_ASSERT_EQUAL(eveningSession.getLightThreshold(), 28);

    /* Настраиваем час начала. */
    for (uint8_t i = 0; i < 17 - 6; i++)
        ui.onMiddlePress();  // уменьшаем час начала с 17 до 6
    ui.onRightPress();  // подтверждаем и переходим к следующему параметру
    TEST_ASSERT_EQUAL(eveningSession.getStartTime().hour(), 6);

    /* Настраиваем минуту начала. */
    for (uint8_t i = 0; i < 20; i++)
        ui.onLeftPress();  // увеличиваем минуты начала с 40 до 60, должны сброситься в 0
    ui.onRightPress();  // подтверждаем и переходим к следующему параметру
    TEST_ASSERT_EQUAL(eveningSession.getStartTime().minute(), 0);

    /* Настраиваем час окончания. */
    for (uint8_t i = 0; i < 10; i++)
        ui.onLeftPress();  // увеличиваем час окончания с 22 до 8, на 24 должен сброситься в 0
    ui.onRightPress();  // подтверждаем и переходим к следующему параметру
    TEST_ASSERT_EQUAL(eveningSession.getEndTime().hour(), 8);

    /* Настраиваем минуту окончания. */
    ui.onMiddlePress();  // уменьшаем минуту окончания с 21 до 20
    ui.onRightPress();  // подтверждаем и выходим в главное меню
    TEST_ASSERT_EQUAL(eveningSession.getEndTime().minute(), 20);

    /* В итоге вечерний сеанс должен стать равным утреннему. */
    TEST_ASSERT_TRUE(eveningSession == morningSession);
}

void processTests()
{
    UNITY_BEGIN();
    RUN_TEST(testRelay);
    RUN_TEST(testButtons);
    RUN_TEST(testClock);
    RUN_TEST(testLightingSession);
    RUN_TEST(testMainMenuUI);
    RUN_TEST(testSessionConfiguringUI);
    UNITY_END();
}

void setup()
{
    processTests();
}

void loop()
{

}
