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
TimerHandle_t buttonHoldTimerHandle;
TimerHandle_t repeatPressTimerHandle;

void checkButtonsTask(void*);
void performLightingTask(void*);
void updateDisplayTask(void*);

void enableUI();
void disableUI(TimerHandle_t);
void startRepeatingPress(TimerHandle_t);
void repeatPress(TimerHandle_t);

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

    buttonHoldTimerHandle = xTimerCreate("btnhld", pdMS_TO_TICKS(1200),
                                         pdFALSE, NULL, startRepeatingPress);

    repeatPressTimerHandle = xTimerCreate("rprss", pdMS_TO_TICKS(200),
                                          pdTRUE, NULL, repeatPress);

    xTimerStart(disableUiTimerHandle, 10);
}

void loop()  // вызывается во время простоя
{
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_mode();
}

void checkButtonsTask(void* unused)
{
    uint8_t events = 0;

    for (;;) {
        events = hardware.getButtonsEvents();

        if (events == 0) {
            xTimerStop(buttonHoldTimerHandle, 10);
            xTimerStop(repeatPressTimerHandle, 10);
        } else if (events & HardwareManager::PRESS_OCCURED_MASK) {
            enableUI();
            if (events & HardwareManager::PRESS_LEFT)
                ui.onLeftPress();
            if (events & HardwareManager::PRESS_MIDDLE)
                ui.onMiddlePress();
            if (events & HardwareManager::PRESS_RIGHT)
                ui.onRightPress();
            xTimerReset(buttonHoldTimerHandle, 10);
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void performLightingTask(void* unused)
{
    DateTime currentTime;
    uint16_t lightLevel;

    // Чтобы автоматическое переключение реле не конфликтовало с ручным.
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
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

void enableUI()
{
    hardware.enableDisplay();
    vTaskResume(updateDisplayTaskHandle);
    xTimerReset(disableUiTimerHandle, 10);
}

void disableUI(TimerHandle_t unused)
{
    hardware.disableDisplay();
    ui.resetMenu();
    vTaskSuspend(updateDisplayTaskHandle);
}

void startRepeatingPress(TimerHandle_t unused)
{
    xTimerReset(repeatPressTimerHandle, 10);
}

void repeatPress(TimerHandle_t unused)
{
    enableUI();
    ui.onPressRepeat();
}
