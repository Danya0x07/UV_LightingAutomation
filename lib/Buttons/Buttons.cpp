#include "Buttons.h"

#define DEBOUNCE_DURATION_MS 5
#define DOUBLEPRESS_SCAN_CYCLES 370

Button::Button(uint8_t pin_, bool inverted_) : DigitalOnePinDevice(pin_, inverted_)
{
    pinMode(pin, inverted_ ? INPUT_PULLUP : INPUT);
    lastState = digitalRead(pin);
}

bool Button::hasBeenPressed()
{
    bool pressed = false;
    bool currentState = digitalRead(pin);

    if (lastState != currentState) {
        delay(DEBOUNCE_DURATION_MS);
        currentState = digitalRead(pin);
    }

    if (lastState && !currentState) pressed = inverted;
    else if (!lastState && currentState) pressed = !inverted;
    lastState = currentState;
    return pressed;
}

bool Button::hasBeenPressedAgain()
{
    for (uint16_t i = 0; i < DOUBLEPRESS_SCAN_CYCLES; i++) {
        delay(1);
        if (hasBeenPressed())
            return true;
    }
    return false;
}
