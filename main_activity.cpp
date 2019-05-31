#include <myButtons.h>
#include <LiquidCrystal.h>
#include <RTClib.h>
#include "main_activity.h"
#include "macro.h"
#include "util.h"
#include "relay.hpp"
#include "data.h"
#include "conf.h"

extern Button btn_rel;
extern Button btn_lcd;
extern Button btn_conf;
extern Relay relay;
extern LiquidCrystal lcd;
extern RTC_DS1307 RTClock;
extern DateTime dt;
extern uint8_t switch_light_value;


void handle_events()
{
    if (btn_rel.pressed()) {
        sound_blink(150, 2);
        relay.switch_state();
    }
    btn_rel.refresh();

    static bool update_disp = true;
    static uint32_t disp_timer = millis();
    
    if (btn_lcd.pressed()) {
        sound_blink(170, 1);
        digitalWrite(LCD_LED_pin, 1);
        disp_timer = millis();
        update_disp = true;
    }
    btn_lcd.refresh();

    if(btn_conf.pressed()) {
        start_configuring();
    }
    btn_conf.refresh();

    if (update_disp) {
        lazy_check_event(update_display, 300);
    }

    if (update_disp && abs(millis() - disp_timer) >= DISPLAY_TIME) {
        update_disp = false;
        lcd.noDisplay();
        digitalWrite(LCD_LED_pin, 0);
    }
}

void watch_period(const uint8_t switch_hour, const uint8_t light_hours, bool& was_light)
{
    if (!was_light && get_light_value() <= switch_light_value 
          && dt.hour() >= switch_hour && dt.hour() < switch_hour + light_hours)
    {
        sound_blink(200, 1);
        relay.set_state(1);
        was_light = true;
        while (dt.hour() < switch_hour + light_hours)
        {
            proceed_in_waiting(handle_events, CHECK_TIME);
            dt = RTClock.now();
        }
        sound_blink(200, 1);
        relay.set_state(0);
        dt = RTClock.now();
    }
}

