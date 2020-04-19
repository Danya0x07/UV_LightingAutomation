#include "test.h"

LightingSession lightingSession;

void setUpTestSession()
{
    lightingSession.setActive(true);
    lightingSession.setLightThreshold(33);
    lightingSession.setStartTime(6, 30);
    lightingSession.setEndTime(9, 0);
}

void testSessionUnderwayDetection()
{
    /* Проверяем рамки яркости. */
    TEST_ASSERT_FALSE(lightingSession.hasToBeUnderway(DateTime(0, 0, 0, 7, 59, 0), 34));
    TEST_ASSERT_TRUE(lightingSession.hasToBeUnderway(DateTime(0, 0, 0, 7, 59, 0), 33));

    /*
     * Проверяем, что однажды достигнув светового порога, уровень освещённости
     * не влияет на дальнейшее прохождение сеанса, то есть сеанс идёт
     * до своего временного конца.
     */
    TEST_ASSERT_TRUE(lightingSession.hasToBeUnderway(DateTime(0, 0, 0, 8, 0, 0), 100));

    /* Проверяем временные рамки. */
    TEST_ASSERT_FALSE(lightingSession.hasToBeUnderway(DateTime(0, 0, 0, 9, 1, 0), 20));
    TEST_ASSERT_FALSE(lightingSession.hasToBeUnderway(DateTime(0, 0, 0, 6, 29, 0), 20));

    /* Проверяем активность/неактивность. */
    lightingSession.setActive(false);
    TEST_ASSERT_FALSE(lightingSession.hasToBeUnderway(DateTime(0, 0, 0, 7, 59, 0), 20));
    lightingSession.setActive(true);
    TEST_ASSERT_TRUE(lightingSession.hasToBeUnderway(DateTime(0, 0, 0, 7, 59, 0), 20));
}

void testSessionSavingLoading()
{
    /*
     * Проверяем корректность сохранения данных о сеансе в EEPROM.
     * для этого сохраняем сеанс по определённому адресу, а потом
     * ещё раз, впритык к уже сохранённым данным. Далее сравниваем
     * загруженный по первоначальному адресу сеанс: если они не равны,
     * значит второе сохранение повредило данные первого, следовательно,
     * LightingSession::getActualEepromPayloadSize() возвращает некорректное значение.
     */
    uint16_t address = 100;
    lightingSession.saveToEeprom(address);
    lightingSession.saveToEeprom(address + LightingSession::getActualEepromPayloadSize());
    LightingSession otherLightingSession;
    otherLightingSession.loadFromEeprom(address);
    TEST_ASSERT_TRUE(otherLightingSession == lightingSession);
}

void testSessionManager()
{
    sessions.morning.setActive(true);
    sessions.morning.setLightThreshold(50);
    sessions.morning.setStartTime(6, 30);
    sessions.morning.setEndTime(8, 0);

    sessions.evening.setActive(true);
    sessions.evening.setLightThreshold(50);
    sessions.evening.setStartTime(20, 0);
    sessions.evening.setEndTime(22, 21);

    /* В 5:49 нет идущих сеансов и ни одного сеанса ещё не было. */
    TEST_ASSERT_EQUAL(0, sessions.check(DateTime(0, 0, 0, 5, 49, 0), 0));
    TEST_ASSERT_EQUAL(0, sessions.getPerformedToday());

    /* В 7:50 идёт только утренний сеанс. */
    TEST_ASSERT_EQUAL(SessionManager::SESSION_MORNING,
                      sessions.check(DateTime(0, 0, 0, 7, 50, 0), 0));

    /* На этот момент суток прошёл только утренний сеанс. */
    TEST_ASSERT_EQUAL(SessionManager::SESSION_MORNING, sessions.getPerformedToday());

    /* В 20:19 идёт только вечерний сеанс. */
    TEST_ASSERT_EQUAL(SessionManager::SESSION_EVENING,
                      sessions.check(DateTime(0, 0, 0, 20, 19, 0), 0));

    /* На этот момент прошли оба сеанса. */
    TEST_ASSERT_EQUAL(SessionManager::SESSION_MORNING | SessionManager::SESSION_EVENING,
                      sessions.getPerformedToday());

    /* В 22:30 нет идущих сеансов, но оба сеанса уже были. */
    TEST_ASSERT_EQUAL(0, sessions.check(DateTime(0, 0, 0, 22, 30, 0), 0));
    TEST_ASSERT_EQUAL(SessionManager::SESSION_MORNING | SessionManager::SESSION_EVENING,
                      sessions.getPerformedToday());

    /* В полночь сеансов нет и прошедшие сеансы сбрасываются. */
    TEST_ASSERT_EQUAL(0, sessions.check(DateTime(0, 0, 0, 0, 0, 0), 0));
    TEST_ASSERT_EQUAL(0, sessions.getPerformedToday());
}
