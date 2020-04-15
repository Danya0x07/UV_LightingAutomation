#include "test.h"

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
     * Поскольку во время сохранения данных вызывается EEPROM.update(),
     * а не EEPROM.write(), ресурс ячеек не расходуется.
     */
    uint16_t address = 100;
    lightingSession.saveToEeprom(address);
    lightingSession.saveToEeprom(address + LightingSession::getActualEepromPayloadSize());
    LightingSession otherLightingSession;
    otherLightingSession.loadFromEeprom(address);
    TEST_ASSERT_TRUE(otherLightingSession == lightingSession);
}
