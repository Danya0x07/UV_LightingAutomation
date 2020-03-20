#pragma once
#include <Arduino.h>

void sound_blink(uint16_t, uint8_t);
void lazy_check_event(void (*)(), uint32_t);
void proceed_in_waiting(void (*)(), uint32_t);

