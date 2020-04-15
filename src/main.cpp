#include <Arduino.h>
#include <LiquidCrystal.h>
#include <RTClib.h>

#include <Arduino_FreeRTOS.h>
#include <timers.h>

#include <HardwareManager.h>
#include <LightingSession.h>
#include <UI.h>

HardwareManager hm;
LightingSession morningSession, eveningSession;
UserInterface ui(hm, &morningSession, &eveningSession);

TaskHandle_t updateDisplayTaskHandle;
TimerHandle_t disableUiTimerHandle;

void checkButtonsTask(void* unused);
void performLightingTask(void* unused);
void updateDisplayTask(void* unused);

void enableUI();
void disableUI(TimerHandle_t unused);

void setup()
{
    LightingSession::setStartAddress(0);
    morningSession.loadFromEeprom();
    eveningSession.loadFromEeprom();

    xTaskCreate(checkButtonsTask, "chbtns", 128,
                NULL, 1, NULL);

    xTaskCreate(performLightingTask, "perfls", 128,
                NULL, 1, NULL);

    xTaskCreate(updateDisplayTask, "updisp", 128,
                NULL, 1, &updateDisplayTaskHandle);

    disableUiTimerHandle = xTimerCreate("uidis", pdMS_TO_TICKS(7000),
                                        pdFALSE, NULL, disableUI);

    xTimerStart(disableUiTimerHandle, 100);
}

void loop()
{
    // TODO: Добавить вход в режим сна.
}

void checkButtonsTask(void* unused)
{
    uint8_t events = 0;

    for (;;)
    {
        events = hm.getPressEvents();

        if (events & HardwareManager::PRESS_WAS) {
            enableUI();
        }
        if (events & HardwareManager::PRESS_LEFT) {
            ui.onLeftPress();
        }
        if (events & HardwareManager::PRESS_MIDDLE) {
            ui.onMiddlePress();
        }
        if (events & HardwareManager::PRESS_RIGHT) {
            ui.onRightPress();
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void performLightingTask(void* unused)
{
    DateTime currentTime;
    uint16_t lightLevel;
    bool relayMustBeEnabled;
    // Чтобы автоматическое переключение реле не конфликтовала с ручным.
    bool relayHasBeenEnabled = false;

    for (;;) {
        currentTime = hm.clock.getTime();
        lightLevel = hm.getLightLevel();
        relayMustBeEnabled = morningSession.hasToBeUnderway(currentTime, (uint8_t)lightLevel) ||
                             eveningSession.hasToBeUnderway(currentTime, (uint8_t)lightLevel);
        if (relayMustBeEnabled) {
            if (relayHasBeenEnabled == false) {
                hm.relay.setState(1);
                relayHasBeenEnabled = true;
            }
        } else {
            if (relayHasBeenEnabled == true) {
                hm.relay.setState(0);
                relayHasBeenEnabled = false;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(30000));
    }
}

void updateDisplayTask(void* unused)
{
    for (;;) {
        ui.updateDisplay(hm.getLightLevel());
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void enableUI()
{
    hm.enableDisplay();
    vTaskResume(updateDisplayTaskHandle);
    xTimerReset(disableUiTimerHandle, 100);
}

void disableUI(TimerHandle_t unused)
{
    hm.disableDisplay();
    ui.resetMenu();
    vTaskSuspend(updateDisplayTaskHandle);
}
