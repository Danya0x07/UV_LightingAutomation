#include <Arduino.h>
#include <LiquidCrystal.h>
#include <RTClib.h>

#include <Arduino_FreeRTOS.h>
#include <timers.h>

#include <Buttons.h>
#include <Relay.h>
#include <Buzzer.h>
#include <LightSensor.h>
#include <Clock.h>
#include <LightingSession.h>

#include "macro.h"

static Button leftButton(10, true); // кнопка, пробуждающая дисплей
static Button middleButton(11, true); // кнопка, переключающая реле
static Button rightButton(9, true);   // кнопка, начинающая сеанс настройки

static Relay relay(A1, true);
static Buzzer buzzer(8, false);
static LightSensor lightSensor(A6);
static LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
static Clock rtclock(A3, A2);
static LightingSession morningSession, eveningSession;

static TaskHandle_t updateDisplayTaskHandle;
static TimerHandle_t displayScreenOffTimer;

static void checkButtons(void* unused);
static void performLightingSessions(void* unused);
static void updateDisplay(void* unused);
static void displayScreenOff(TimerHandle_t unused);

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

    xTaskCreate(checkButtons, "chbtns", 128,
                NULL, 1, NULL);

    xTaskCreate(performLightingSessions, "perfls", 128,
                NULL, 1, NULL);

    xTaskCreate(updateDisplay, "updisp", 128,
                NULL, 1, &updateDisplayTaskHandle);

    displayScreenOffTimer = xTimerCreate("tdsoff", pdMS_TO_TICKS(7000),
                                         pdFALSE, NULL, displayScreenOff);
                                         
    xTimerStart(displayScreenOffTimer, 100);
}

void loop()
{
    
}

static void checkButtons(void* unused)
{

    for (;;)
    {
        bool lpress = leftButton.hasBeenPressed();
        bool mpress = middleButton.hasBeenPressed();
        bool rpress = rightButton.hasBeenPressed();

        if (lpress || mpress || rpress) {
            lcd.backlight();
            lcd.display();
            vTaskResume(updateDisplayTaskHandle);
            xTimerReset(displayScreenOffTimer, 100);
        }

        if (lpress) {
        }

        if (mpress) {
        }

        if (rpress) {
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

static void performLightingSessions(void* unused)
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

static void updateDisplay(void* unused)
{

    for (;;) {

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

static void displayScreenOff(TimerHandle_t unused)
{
    lcd.noBacklight();
    lcd.noDisplay();
    vTaskSuspend(updateDisplayTaskHandle);
}
