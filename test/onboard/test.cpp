#include "test.h"

HardwareManager hardware;
SessionManager sessions;
UserInterface ui(hardware, sessions);

void processTests()
{
    UNITY_BEGIN();
    RUN_TEST(testRelay);
    RUN_TEST(testButtons);
    RUN_TEST(testClock);
    RUN_TEST(testSessionUnderwayDetection);
    RUN_TEST(testSessionSavingLoading);
    RUN_TEST(testMenuTransitions);
    RUN_TEST(testMainMenuUI);
    RUN_TEST(testSessionConfiguringUI);
    RUN_TEST(testClockConfiguringUI);
    UNITY_END();
}

void setup()
{
    setUpTestSession();
    processTests();
}

void loop()
{
}
