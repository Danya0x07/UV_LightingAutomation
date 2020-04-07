#include <Arduino.h>
#include <LiquidCrystal.h>
#include <RTClib.h>

#include <Arduino_FreeRTOS.h>
#include <timers.h>

#include <Buttons.h>
#include <Relay.h>
#include <Buzzer.h>
#include <Clock.h>
#include <LightSensor.h>
#include <LightingSession.h>
#include <UI.h>

#include "macro.h"

Button leftButton(10, true);   // кнопка, пробуждающая дисплей
Button middleButton(11, true); // кнопка, переключающая реле
Button rightButton(9, true);   // кнопка, начинающая сеанс настройки
Relay relay(A1, true);
Buzzer buzzer(8, false);
Clock rtclock(A3, A2);
LightSensor lightSensor(A6);
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
LightingSession morningSession, eveningSession;

TaskHandle_t updateDisplayTaskHandle;
TimerHandle_t disableDisplayTimerHandle;

void checkButtonsTask(void* unused);
void performLightingTask(void* unused);
void updateDisplayTask(void* unused);
void enableDisplay();
void disableDisplay(TimerHandle_t unused);

void setup()
{

    pinMode(CONN_LED_pin, OUTPUT);
    digitalWrite(CONN_LED_pin, 0);
    pinMode(13, OUTPUT);  // минус для зелёного светика
    digitalWrite(13, 0);

    lcd.setBacklightPin(12, POSITIVE);
    lcd.begin(16, 2);

    morningSession.loadFromEeprom(0 * LightingSession::getActualEepromPayloadSize());
    eveningSession.loadFromEeprom(1 * LightingSession::getActualEepromPayloadSize());

    xTaskCreate(checkButtonsTask, "chbtns", 128,
                NULL, 1, NULL);

    xTaskCreate(performLightingTask, "perfls", 128,
                NULL, 1, NULL);

    xTaskCreate(updateDisplayTask, "updisp", 128,
                NULL, 1, &updateDisplayTaskHandle);

    disableDisplayTimerHandle = xTimerCreate("uidis", pdMS_TO_TICKS(7000),
                                        pdFALSE, NULL, disableDisplay);

    xTimerStart(disableDisplayTimerHandle, 100);
}

void loop()
{
    // TODO: Добавить вход в режим сна.
}

void checkButtonsTask(void* unused)
{
    for (;;)
    {
        bool leftPress = leftButton.hasBeenPressed();
        bool middlePress = middleButton.hasBeenPressed();
        bool rightPress = rightButton.hasBeenPressed();

        if (leftPress || middlePress || rightPress) {
            enableDisplay();
        }

        if (leftPress) {
        }

        if (middlePress) {
        }

        if (rightPress) {
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void performLightingTask(void* unused)
{
    DateTime currentTime;
    uint16_t lightLevel;
    bool relayMustBeEnabled;
    bool relayHasBeenEnabled = false; // Чтобы автоматическое переключение реле не конфликтовала с ручным.

    for (;;) {
        currentTime = rtclock.getTime();
        lightLevel = lightSensor.getValue();
        relayMustBeEnabled = morningSession.hasToBeUnderway(currentTime, (uint8_t)lightLevel) ||
                             eveningSession.hasToBeUnderway(currentTime, (uint8_t)lightLevel);
        if (relayMustBeEnabled) {
            if (relayHasBeenEnabled == false) {
                relay.setState(1);
                relayHasBeenEnabled = true;
            }
        }
        else {
            if (relayHasBeenEnabled == true) {
                relay.setState(0);
                relayHasBeenEnabled = false;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(30000));
    }
}

void updateDisplayTask(void* unused)
{

    for (;;) {

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void enableDisplay()
{
    lcd.backlight();
    lcd.display();
    vTaskResume(updateDisplayTaskHandle);
    xTimerReset(disableDisplayTimerHandle, 100);
}

void disableDisplay(TimerHandle_t unused)
{
    lcd.noBacklight();
    lcd.noDisplay();
    vTaskSuspend(updateDisplayTaskHandle);
}
