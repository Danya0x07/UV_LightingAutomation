#include "Relay.h"

Relay::Relay(uint8_t pin, bool inverted = false) : pin(pin), inverted(inverted)
{
    pinMode(pin, OUTPUT);
    setState(0);
}