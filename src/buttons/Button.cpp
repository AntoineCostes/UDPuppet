#include "Button.h"

Button::Button(int pin, long longPressMs): Component("btn_" + String(pin)), 
                                        pin(pin), 
                                        isPressed(false),
                                        longPressMs(longPressMs), 
                                        lastPressMs(0),
                                        isLongPressed(true)
{
}

void Button::initComponent(bool serialDebug)
{
    Component::initComponent(serialDebug);
    pinMode(pin, INPUT_PULLUP);
}

void Button::update()
{
    bool currentState = !digitalRead(pin);
    if (isPressed != currentState)
    {
        if (currentState)
        {
            compDebug("PRESSED");
            sendEvent(ButtonEvent(pin, ButtonEvent::Type::PRESSED));
            isPressed = true;
            isLongPressed = false;
            lastPressMs = millis();
        }
        else
        {
            compDebug("RELEASED");
            if (millis() - lastPressMs < longPressMs)
                sendEvent(ButtonEvent(pin, ButtonEvent::Type::RELASED_SHORT));
            else
                sendEvent(ButtonEvent(pin, ButtonEvent::Type::RELEASED_LONG));
            
            isPressed = false;
            isLongPressed = false;
        }

    } else if (isPressed && !isLongPressed && millis() - lastPressMs > longPressMs)
    {
        compDebug("MAINTAINED");
        sendEvent(ButtonEvent(pin, ButtonEvent::Type::LONG_PRESS));
        isLongPressed = true;
    }
}