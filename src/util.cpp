#include "util.h"
#include "macro.h"

void sound_blink(uint16_t howlong, uint8_t howmany)
{
    for (uint8_t i = 0; i < howmany; i++)
    {
        digitalWrite(BUZZ_pin, 1);
        delay(howlong);
        digitalWrite(BUZZ_pin, 0);
        delay(howlong / 2);
    }
}

void lazy_check_event(void (*oper)(), uint32_t check_timer)
{
    static uint32_t ask_timer = 0;
    if (abs(millis() - ask_timer) >= check_timer)
    {
        oper();
        ask_timer = millis();
    }
}

void proceed_in_waiting(void (*oper)(), uint32_t waiting_timer)
{
    uint32_t ask_timer = millis();
    while (abs(millis() - ask_timer) < waiting_timer)
    {
        oper();
    }
}

