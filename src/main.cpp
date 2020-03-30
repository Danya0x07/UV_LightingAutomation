#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <LiquidCrystal.h>
#include <RTClib.h>

#include <Buttons.h>
#include <Relay.h>
#include <Buzzer.h>
#include <LightSensor.h>
#include <Clock.h>
#include <LightingSession.h>

#include "macro.h"

Button leftButton(10, true);    // кнопка, пробуждающая дисплей
Button middleButton(11, true);  // кнопка, переключающая реле
Button rightButton(9, true);    // кнопка, начинающая сеанс настройки
Relay relay(A1, true);
Buzzer buzzer(8, false);
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
Clock rtclock(A3, A2);
DateTime dt;

void taskCheckButtons(const void* unused)
{

    for(;;) {
        if (leftButton.hasBeenPressed()) {

        }

        if (middleButton.hasBeenPressed()) {

        }

        if (rightButton.hasBeenPressed()) {

        }

        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

void setup()
{    
    
    pinMode(CONN_LED_pin, OUTPUT);
    digitalWrite(CONN_LED_pin, 0);
    pinMode(13, OUTPUT);  // минус для зелёного светика
    digitalWrite(13, 0);
    

    lcd.setBacklightPin(12, POSITIVE);
    lcd.begin(16, 2);
    lcd.print("Time:");
    lcd.setCursor(0, 1);
    lcd.print("Light:");
    //update_display();
}

void loop()
{
    
}
