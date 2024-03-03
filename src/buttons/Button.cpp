#include "Button.h"

Button::Button(int pin, long shortPressMs, long longPressMs): Component("btn_" + String(pin)), 
                                        pin(pin), 
                                        isPressed(false),
                                        shortPressMs(shortPressMs),
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
        if (currentState && millis() - lastPressMs > shortPressMs)
        {
            compDebug("PRESSED");
            sendEvent(ButtonEvent(pin, ButtonEvent::Type::PRESSED));
            isPressed = true;
            isLongPressed = false;
            lastPressMs = millis();
        }
        else if (millis() - lastPressMs > shortPressMs)
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