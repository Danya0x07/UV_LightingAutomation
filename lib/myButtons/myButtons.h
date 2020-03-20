#ifndef MYBUTTONS_H
#define MYBUTTONS_H

#include <Arduino.h>

class Button
{
protected:
    const uint8_t pin;
    const bool mode;

    bool debounce();
    bool LastBut;
    bool CurBut;

public:
    Button(uint8_t, bool);
    bool is;
    bool pressed();
    void refresh();
};


template <class... Args>
class SmartButton : public Button
{
protected:
    using funcptr_t = void (*)(Args&... args);
    funcptr_t handler;

public:
    SmartButton(uint8_t pin, bool mode, funcptr_t hdl=nullptr)
        : Button(pin, mode), handler(hdl)
    {
    }

    void set_handler(funcptr_t handler)
    {
        this->handler = handler;
    }

    void handle_press(Args&... args)
    {
        if (pressed() && handler)
        {
            handler(args...);
        }
        refresh();
    }
};

#endif
