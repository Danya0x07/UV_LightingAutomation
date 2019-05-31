#include <myButtons.h>
#include <RTClib.h>
#include <LiquidCrystal.h>
#include "relay.hpp"
#include "macro.h"
#include "data.h"
#include "main_activity.h"
#include "util.h"

Button btn_rel(BTN_REL_pin, true);
Button btn_lcd(BTN_LCD_pin, true);
Button btn_conf(BTN_CONF_pin, true);
Relay relay(RELAY_pin, true);
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
RTC_DS1307 RTClock;
DateTime dt;

uint8_t switch_light_value  = 35;  // Порог освещённости
uint8_t morning_switch_hour = 6;  // Время утренней досветки
uint8_t evening_switch_hour = 19; // Время вечерней досветки
uint8_t morning_light_hours = 2;  // Длительность утренней досветки
uint8_t evening_light_hours = 2;  // Длительность вечерней досветки

bool was_morning_light = false;
bool was_evening_light = false;

void setup()
{
    relay.set_state(0);
    
    pinMode(RTC_POW_pin, OUTPUT);
    pinMode(RTC_GND_pin, OUTPUT);
    pinMode(LCD_LED_pin, OUTPUT);
    pinMode(BUZZ_pin, OUTPUT);
    pinMode(CONN_LED_pin, OUTPUT);
    pinMode(13, OUTPUT);  // минус для зелёного светика
    
    digitalWrite(RTC_POW_pin, 1);
    digitalWrite(RTC_GND_pin, 0);
    digitalWrite(LCD_LED_pin, 1);
    digitalWrite(13, 0);
    
    RTClock.begin();
    lcd.begin(16, 2);
    lcd.print("Time:");
    lcd.setCursor(0, 1);
    lcd.print("Light:");
    update_display();
}

void loop()
{
    dt = RTClock.now();

    watch_period(morning_switch_hour, morning_light_hours, was_morning_light);
    watch_period(evening_switch_hour, evening_light_hours, was_evening_light);

    if (dt.hour() == 0)
    {
        was_morning_light = false;
        was_evening_light = false;
    }

    proceed_in_waiting(handle_events, CHECK_TIME);
}
