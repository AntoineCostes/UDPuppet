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
        compDebug("===================");
        if (currentState) compDebug("currentState TRUE");
        else compDebug("currentState FALSE");
        
        if (isPressed) compDebug("isPressed TRUE");
        else compDebug("isPressed FALSE");

        if (currentState)
        {
            sendEvent(ButtonEvent(pin, ButtonEvent::Type::PRESSED));
            isPressed = true;
            isLongPressed = false;
            lastPressMs = millis();
        }
        else
        {
            if (millis() - lastPressMs < longPressMs)
                sendEvent(ButtonEvent(pin, ButtonEvent::Type::RELASED_SHORT));
            else
                sendEvent(ButtonEvent(pin, ButtonEvent::Type::RELEASED_LONG));
            
            isPressed = false;
        }
        if (isPressed) compDebug("currentState TRUE");
        else compDebug("currentState FALSE");

    } else if (!isLongPressed && millis() - lastPressMs > longPressMs)
    {
        sendEvent(ButtonEvent(pin, ButtonEvent::Type::LONG_PRESS));
        isLongPressed = true;
    }
}