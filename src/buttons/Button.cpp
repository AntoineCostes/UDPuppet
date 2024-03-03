#include "Button.h"

Button::Button(int pin, long shortPressMs, long longPressMs, 
bool clearOnPressed, bool playSequencesOnShort, 
bool cancelSoundOnLongPress, bool enableHotspotOnLong): Component("btn_" + String(pin)), 
                                        pin(pin), 
                                        isPressed(false),
                                        shortPressMs(shortPressMs),
                                        longPressMs(longPressMs), 
                                        clearOnPressed(clearOnPressed),
                                        playSequencesOnShort(playSequencesOnShort),
                                        cancelSoundOnLongPress(cancelSoundOnLongPress),
                                        enableHotspotOnLong(enableHotspotOnLong),
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
            sendEvent(ButtonEvent(ButtonEvent::Type::PRESSED, clearOnPressed, playSequencesOnShort, cancelSoundOnLongPress, enableHotspotOnLong));
            isPressed = true;
            isLongPressed = false;
            lastPressMs = millis();
        }
        else if (millis() - lastPressMs > shortPressMs)
        {
            compDebug("RELEASED");
            if (millis() - lastPressMs < longPressMs)
                sendEvent(ButtonEvent(ButtonEvent::Type::RELASED_SHORT, clearOnPressed, playSequencesOnShort, cancelSoundOnLongPress, enableHotspotOnLong));
            else
                sendEvent(ButtonEvent(ButtonEvent::Type::RELEASED_LONG, clearOnPressed, playSequencesOnShort, cancelSoundOnLongPress, enableHotspotOnLong));
            
            isPressed = false;
            isLongPressed = false;
        }

    } else if (isPressed && !isLongPressed && millis() - lastPressMs > longPressMs)
    {
        compDebug("MAINTAINED");
        sendEvent(ButtonEvent(ButtonEvent::Type::LONG_PRESS, clearOnPressed, playSequencesOnShort, cancelSoundOnLongPress, enableHotspotOnLong));
        isLongPressed = true;
    }
}