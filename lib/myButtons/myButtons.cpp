#include "myButtons.h"

Button::Button(uint8_t pin, bool mode) : pin(pin), mode(mode)
{
    pinMode(pin, mode ? INPUT_PULLUP : INPUT);
    is = true;
    LastBut = digitalRead(pin);
}

bool Button::debounce()
{
    bool current = digitalRead(pin);
    if(LastBut != current)
    {
        delay(5);
        current = digitalRead(pin);
    }
    return current;
}

bool Button::pressed()
{ 
    if(!is) return false;
    CurBut = debounce();
    if(LastBut && !CurBut) return mode;
    else if(!LastBut && CurBut) return !mode;
    else return false; 
}

void Button::refresh()
{
    LastBut = CurBut;
}

