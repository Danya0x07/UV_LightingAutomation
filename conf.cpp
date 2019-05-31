#include "conf.h"
#include "macro.h"
#include "util.h"
#include <Arduino.h>
#include <myButtons.h>
#include <RTClib.h>

extern Button btn_conf;
extern RTC_DS1307 RTClock;
extern uint8_t switch_light_value;
extern uint8_t morning_switch_hour;  
extern uint8_t evening_switch_hour; 
extern uint8_t morning_light_hours;
extern uint8_t evening_light_hours;

static uint8_t* params[NUM_OF_PARAMS] = {
    &switch_light_value,
    &morning_switch_hour,
    &evening_switch_hour,
    &morning_light_hours,
    &evening_light_hours
};

static void get_new_params();
static void send_params();
static void configure_rtc();

void start_configuring()
{
    digitalWrite(CONN_LED_pin, 1);
    Serial.begin(9600);
    sound_blink(300, 1);
    while (!btn_conf.pressed())
    {
        btn_conf.refresh();
        if (Serial.available()) {
            char command = Serial.read();
            if (command == 's') {
                get_new_params();
            } else if (command == 'g') {
                send_params();
            } else if (command == 't') {
                configure_rtc();
            }
            sound_blink(140, 1);
        }
    }
    Serial.end();
    digitalWrite(CONN_LED_pin, 0);
    sound_blink(300, 1);
}

static void get_new_params()
{
    while (Serial.available() < NUM_OF_PARAMS);
    for (uint8_t* param: params) {
        *param = Serial.read();
    }
}

static void send_params()
{
    for (uint8_t* param: params) {
        Serial.write(*param);
    }
}

static void configure_rtc()
{
    while (Serial.available() < 5);
    uint8_t month = Serial.read();
    uint8_t day   = Serial.read();
    uint8_t hour  = Serial.read();
    uint8_t min   = Serial.read();
    uint8_t sec   = Serial.read();
    RTClock.adjust(DateTime(18ul, month, day, hour, min, sec));
}
