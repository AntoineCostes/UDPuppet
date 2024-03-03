
#pragma once
#include "../common/Component.h"
#include "../utils/EventBroadcaster.h"


class ButtonEvent
{
public:
    enum Type
    {
        PRESSED,
        LONG_PRESS,
        RELASED_SHORT,
        RELEASED_LONG
    } type;

    UDPuppetButtonBehavior behavior;
    ButtonEvent(Type type, UDPuppetButtonBehavior behavior) : type(type), behavior(behavior){}
};

class Button : public Component,
                public EventBroadcaster<ButtonEvent>
{
public:
    Button(int pin, long shortPressMs, long longPressMs, UDPuppetButtonBehavior behavior);

    void initComponent(bool serialDebug);
    void update() override;
    
    void pressed();
    void released();

    UDPuppetButtonBehavior behavior;
    
protected:
    int pin;
    bool isPressed;
    long shortPressMs;
    long longPressMs;
    bool isLongPressed;
    long lastPressMs;
};
