
#pragma once
#include "../common/Component.h"
#include "../utils/EventBroadcaster.h"


class ButtonEvent
{
public:
    int pin;
    enum Type
    {
        PRESSED,
        LONG_PRESS,
        RELASED_SHORT,
        RELEASED_LONG
    } type;
    ButtonEvent(int pin, Type type) : pin(pin), type(type) {}
};

class Button : public Component,
                public EventBroadcaster<ButtonEvent>
{
public:
    Button(int pin, long longPressMs);

    void initComponent(bool serialDebug);
    void update();
    
    void pressed();
    void released();


protected:
    int pin;
    bool isPressed;
    long longPressMs;
    bool isLongPressed;
    long lastPressMs;
};
