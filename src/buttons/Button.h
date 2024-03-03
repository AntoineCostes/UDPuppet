
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

    bool clearOnPressed;
    bool playSequencesOnShort;
    bool cancelSoundOnLongPress;
    bool enableHotspotOnLong;

    ButtonEvent(Type type, 
    bool clearOnPressed, bool playSequencesOnShort, bool cancelSoundOnLongPress, bool enableHotspotOnLong) : type(type), 
                                        clearOnPressed(clearOnPressed),
                                        playSequencesOnShort(playSequencesOnShort),
                                        cancelSoundOnLongPress(cancelSoundOnLongPress),
                                        enableHotspotOnLong(enableHotspotOnLong) {}
};

class Button : public Component,
                public EventBroadcaster<ButtonEvent>
{
public:
    Button(int pin, long shortPressMs, long longPressMs, bool clearOnPressed, bool playSequencesOnShort, bool cancelSoundOnLongPress, bool enableHotspotOnLong);

    void initComponent(bool serialDebug);
    void update() override;
    
    void pressed();
    void released();

    bool clearOnPressed;
    bool playSequencesOnShort;
    bool cancelSoundOnLongPress;
    bool enableHotspotOnLong;
    
protected:
    int pin;
    bool isPressed;
    long shortPressMs;
    long longPressMs;
    bool isLongPressed;
    long lastPressMs;
};
