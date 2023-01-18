#pragma once
#include "Button.h"
#include "../common/Manager.h"
#include "../utils/EventBroadcaster.h"


class ButtonManager : public Manager,
                       public EventBroadcaster<ButtonEvent>
{
public:
    ButtonManager();

    void initManager();
    void update();
    void registerButton(int pin);
    void gotButtonEvent(const ButtonEvent &e);

protected:
    std::vector<Button*> buttons; 
};
