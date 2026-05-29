#include "Button.h"

Button::Button(int pin, long shortPressMs, long longPressMs, UDPuppetButtonBehavior behavior): Component("btn_" + String(pin)), 
                                        pin(pin), 
                                        isPressed(false),
                                        shortPressMs(shortPressMs),
                                        longPressMs(longPressMs), 
                                        behavior(behavior),
                                        lastPressMs(0),
                                        isLongPressed(true)
{
}

void Button::initComponent(bool serialDebug)
{
    Component::initComponent(serialDebug);
    pinMode(pin, INPUT_PULLUP);
    
    isPressed =!digitalRead(pin);
}

void Button::update()
{
    bool currentState = !digitalRead(pin);

    if (isPressed != currentState)
    {
        if (currentState && millis() - lastPressMs > shortPressMs)
        {
            compDebug("PRESSED");
            sendEvent(ButtonEvent(ButtonEvent::Type::PRESSED, behavior));
            isPressed = true;
            isLongPressed = false;
            lastPressMs = millis();
        }
        else if (millis() - lastPressMs > shortPressMs)
        {
            compDebug("RELEASED");
            if (millis() - lastPressMs < longPressMs)
                sendEvent(ButtonEvent(ButtonEvent::Type::RELASED_SHORT, behavior));
            else
                sendEvent(ButtonEvent(ButtonEvent::Type::RELEASED_LONG, behavior));
            
            isPressed = false;
            isLongPressed = false;
        }

    } else if (isPressed && !isLongPressed && millis() - lastPressMs > longPressMs)
    {
        compDebug("MAINTAINED");
        sendEvent(ButtonEvent(ButtonEvent::Type::LONG_PRESS, behavior));
        isLongPressed = true;
    }
}