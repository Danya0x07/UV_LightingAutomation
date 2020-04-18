#include <Arduino.h>
#include <avr/sleep.h>

#include <Arduino_FreeRTOS.h>
#include <timers.h>

#include <HardwareManager.h>
#include <SessionManager.h>
#include <UI.h>

HardwareManager hardware;
SessionManager sessions;
UserInterface ui(hardware, sessions);

TaskHandle_t updateDisplayTaskHandle;
TimerHandle_t disableUiTimerHandle;

void checkButtonsTask(void* unused);
void performLightingTask(void* unused);
void updateDisplayTask(void* unused);

void enableUI();
void disableUI(TimerHandle_t unused);

void setup()
{
    hardware.createDisplay();
    hardware.enableDisplay();
    ui.resetMenu();

    xTaskCreate(checkButtonsTask, "chbtns", 128,
                NULL, 1, NULL);

    xTaskCreate(performLightingTask, "perfls", 128,
                NULL, 1, NULL);

    xTaskCreate(updateDisplayTask, "updisp", 128,
                NULL, 1, &updateDisplayTaskHandle);

    disableUiTimerHandle = xTimerCreate("uidis", pdMS_TO_TICKS(10000),
                                        pdFALSE, NULL, disableUI);

    xTimerStart(disableUiTimerHandle, 100);
}

void loop()  // вызывается во время простоя
{
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_mode();
}

void checkButtonsTask(void* unused)
{
    uint8_t events = 0;

    for (;;)
    {
        events = hardware.getPressEvents();

        if (events != 0) {
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

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void performLightingTask(void* unused)
{
    DateTime currentTime;
    uint16_t lightLevel;
    // Чтобы автоматическое переключение реле не конфликтовала с ручным.
    bool relayHasBeenEnabled = false;

    for (;;) {
        currentTime = hardware.clock.getTime();
        lightLevel = hardware.getLightLevel();

        if (sessions.check(currentTime, lightLevel) != 0) {
            if (relayHasBeenEnabled == false) {
                hardware.relay.setState(1);
                relayHasBeenEnabled = true;
            }
        } else {
            if (relayHasBeenEnabled == true) {
                hardware.relay.setState(0);
                relayHasBeenEnabled = false;
            }
        }

        vTaskDelay(pdMS_TO_TICKS(30000));
    }
}

void updateDisplayTask(void* unused)
{
    for (;;) {
        ui.updateDisplay();
        vTaskDelay(pdMS_TO_TICKS(333));
    }
}

void enableUI()
{
    hardware.enableDisplay();
    vTaskResume(updateDisplayTaskHandle);
    xTimerReset(disableUiTimerHandle, 100);
}

void disableUI(TimerHandle_t unused)
{
    hardware.disableDisplay();
    ui.resetMenu();
    vTaskSuspend(updateDisplayTaskHandle);
}
