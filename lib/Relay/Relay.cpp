#include "Relay.h"

Relay::Relay(uint8_t pin, bool inverse = false) : pin(pin), inverse(inverse)
{
    pinMode(pin, OUTPUT);
    setState(0);
}