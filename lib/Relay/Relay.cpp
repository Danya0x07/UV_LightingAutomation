#include "Relay.h"

Relay::Relay(uint8_t pin, bool inverted = false) : pin(pin), inverted(inverted)
{
    pinMode(pin, OUTPUT);

    /* Выходы для зелёного светодиода, показывающего, замкнуто ли реле. */
    pinMode(A0, OUTPUT);  // VCC
    digitalWrite(A0, 0);
    pinMode(13, OUTPUT);  // GND
    digitalWrite(13, 0);

    setState(0);
}

void Relay::setState(bool state)
{
    digitalWrite(pin, state != inverted);
    digitalWrite(A0, state);
}