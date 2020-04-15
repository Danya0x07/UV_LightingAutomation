#include "test.h"

HardwareManager hardware;

void processTests()
{
    UNITY_BEGIN();
    RUN_TEST(testRelay);
    RUN_TEST(testButtons);
    RUN_TEST(testClock);
    RUN_TEST(testLightingSession);
    RUN_TEST(testMainMenuUI);
    RUN_TEST(testSessionConfiguringUI);
    RUN_TEST(testClockConfiguringUI);
    UNITY_END();
}

void setup()
{
    processTests();
}

void loop()
{
}
