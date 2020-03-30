#include "LightSensor.h"

uint16_t LightSensor::getValue(uint16_t limit)
{
    uint16_t value = analogRead(pin);
    value = map(value, 0, 1023, 0, limit);
    return value;
}