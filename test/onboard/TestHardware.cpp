#include "test.h"

void testRelay()
{
    hardware.relay.setState(0);
    TEST_ASSERT_EQUAL(hardware.relay.getState(), false);
    hardware.relay.setState(1);
    TEST_ASSERT_EQUAL(hardware.relay.getState(), true);
    delay(700);  // реле лучше резко не переключать
    hardware.relay.switchState();
    TEST_ASSERT_EQUAL(hardware.relay.getState(), false);
}

void testButtons()
{
    /*
     * В состоянии покоя кнопки не должны быть нажаты,
     * если это не так, значит внутренние подтягивающие
     * резисторы вышли из строя.
     */
    TEST_ASSERT_FALSE(hardware.leftButton.isPressed());
    TEST_ASSERT_FALSE(hardware.middleButton.isPressed());
    TEST_ASSERT_FALSE(hardware.rightButton.isPressed());
}

void testClock()
{
#ifdef DEVBOARD
    /* На отладочной плате нет подключённых RTC. */
    TEST_IGNORE_MESSAGE("There is no RTC connected to devboard.");
#endif
    DateTime dt = hardware.clock.getTime();
    delay(1001);
    /* Проверяем, что часы идут. */
    TEST_ASSERT_TRUE(hardware.clock.getTime() > dt);
}
