#include "Relay.h"

Relay::Relay(uint8_t pin_, bool inverted_) : DigitalOnePinDevice(pin_, inverted_)
{
    pinMode(pin_, OUTPUT);

    /* Выходы для зелёного светодиода, показывающего, замкнуто ли реле. */
    pinMode(A0, OUTPUT);  // VCC
    digitalWrite(A0, 0);
    pinMode(13, OUTPUT);  // GND
    digitalWrite(13, 0);

    setState(0);
}

void Relay::setState(bool state)
{
    DigitalOnePinDevice::setState(state);
    digitalWrite(A0, state);
}
