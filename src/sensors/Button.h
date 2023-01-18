
#pragma once
#include "../common/Component.h"
#include "../utils/EventBroadcaster.h"

#include <EasyButton.h>


class ButtonEvent
{
public:
    int pin;
    enum Type
    {
        PRESSED,
        RELASED_SHORT,
        RELEASED_LONG
    } type;
    ButtonEvent(int pin, Type type) : pin(pin), type(type) {}
};

class Button : public Component,
                public EventBroadcaster<ButtonEvent>
{
public:
    Button(int pin);

    void initComponent(bool serialDebug);
    void update();
    
    void pressed();


protected:
    int pin;
    EasyButton button;
};
