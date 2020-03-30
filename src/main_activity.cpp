#include <Buttons.h>
#include <LiquidCrystal.h>
#include <RTClib.h>
#include "main_activity.h"
#include "macro.h"
#include "util.h"
#include "Relay.h"
#include "data.h"
#include "conf.h"

extern Button relayButton;
extern Button displayButton;
extern Button configButton;
extern Relay relay;
extern LiquidCrystal lcd;
extern RTC_DS1307 RTClock;
extern DateTime dt;
extern uint8_t switch_light_value;


void handle_events()
{
    static bool update_disp = true;
    static uint32_t disp_timer = millis();

    if (relayButton.hasBeenPressed()) {
        sound_blink(150, 2);
        relay.switchState();
    }


    if (displayButton.hasBeenPressed()) {
        sound_blink(170, 1);
        digitalWrite(LCD_LED_pin, 1);
        disp_timer = millis();
        update_disp = true;
    }

    if (configButton.hasBeenPressed()) {
        start_configuring();
    }

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
        relay.setState(1);
        was_light = true;
        while (dt.hour() < switch_hour + light_hours)
        {
            proceed_in_waiting(handle_events, CHECK_TIME);
            dt = RTClock.now();
        }
        sound_blink(200, 1);
        relay.setState(0);
        dt = RTClock.now();
    }
}
