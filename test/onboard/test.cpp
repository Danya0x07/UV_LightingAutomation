#include <Arduino.h>
#include <unity.h>

#include <Relay.h>
#include <Buttons.h>
#include <Clock.h>
#include <LightingSession.h>

static Relay relay(A1, true);
static Button leftButton(10, true),
              middleButton(11, true),
              rightButton(9, true);
static Clock clock(A3, A2);
static LightingSession lightingSession;

static void testRelay()
{
    relay.setState(0);
    TEST_ASSERT_EQUAL(relay.getState(), false);
    relay.setState(1);
    TEST_ASSERT_EQUAL(relay.getState(), true);
    delay(500);  // реле лучше резко не переключать
    relay.switchState();
    TEST_ASSERT_EQUAL(relay.getState(), false);
}

static void testButtons()
{
    TEST_ASSERT_FALSE(leftButton.isPressed());
    TEST_ASSERT_FALSE(middleButton.isPressed());
    TEST_ASSERT_FALSE(rightButton.isPressed());
}

static void testClock()
{
#ifdef DEVBOARD
    /* На отладочной плате нет подключённых RTC. */
    TEST_IGNORE();
#endif
    DateTime dt = clock.getTime();
    delay(1001);
    /* Проверяем, что часы идут. */
    TEST_ASSERT_TRUE(clock.getTime() > dt);
}

static void testLightingSession()
{
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

void processTests()
{
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(testRelay);
    RUN_TEST(testButtons);
    RUN_TEST(testClock);
    RUN_TEST(testLightingSession);
    UNITY_END();
}

void setup()
{
    processTests();
}

void loop()
{

}
