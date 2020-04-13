#include "Buttons.h"

#define DEBOUNCE_DURATION_MS 5
#define DOUBLEPRESS_SCAN_CYCLES 370

Button::Button(uint8_t pin, bool inverted) : pin(pin), inverted(inverted)
{
    pinMode(pin, inverted ? INPUT_PULLUP : INPUT);
    lastState = digitalRead(pin);
}

bool Button::isPressed()
{
    return bool(digitalRead(pin)) != inverted;
}

bool Button::hasBeenPressed()
{ 
    bool pressed = false;
    bool currentState = isPressed();
    if (lastState != currentState) {
        delay(DEBOUNCE_DURATION_MS);
        currentState = isPressed();
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
