#include "Buttons.h"

#define DEBOUNCE_DURATION_MS 5
#define DOUBLEPRESS_SCAN_CYCLES 370

Button::Button(uint8_t pin, bool inverse) : pin(pin), inverse(inverse)
{
    pinMode(pin, inverse ? INPUT_PULLUP : INPUT);
    lastState = digitalRead(pin);
}

bool Button::isPressed()
{
    return digitalRead(pin) != inverse;
}

bool Button::hasBeenPressed()
{ 
    bool pressed = false;
    bool currentState = isPressed();
    if (lastState != currentState) {
        delay(DEBOUNCE_DURATION_MS);
        currentState = isPressed();
    }

    if (lastState && !currentState) pressed = inverse;
    else if (!lastState && currentState) pressed = !inverse;
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
