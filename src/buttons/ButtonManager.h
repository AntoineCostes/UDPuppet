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
    void registerButton(int pin, long longPressDuration);
    void gotButtonEvent(const ButtonEvent &e);

protected:
    std::vector<Button*> buttons; 
};
