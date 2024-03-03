#pragma once
#include "Button.h"
#include "../common/Manager.h"
#include "../utils/EventBroadcaster.h"


class ButtonManager : public Manager,
                       public EventBroadcaster<ButtonEvent>
{
public:
    ButtonManager();

    void initManager() override;
    void update() override;
    void registerButton(int pin, long shortPressDuration, long longPressDuration, bool clearOnPressed, bool playSequencesOnShort, bool cancelSoundOnLongPress, bool enableHotspotOnLong);
    void gotButtonEvent(const ButtonEvent &e);

protected:
    std::vector<Button*> buttons; 
};
