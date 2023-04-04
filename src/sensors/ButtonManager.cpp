#include "ButtonManager.h"


ButtonManager::ButtonManager() : Manager("Button")
{
    serialDebug = MASTER_DEBUG;
}

void ButtonManager::initManager()
{
    Manager::initManager();
}

void ButtonManager::registerButton(int pin, long longPressDuration)
{
    if (!checkInit())
        return;

    if (!Component::registerPin(pin))
    {
        compError("cannot register button: this pin is already registered !");
        return;
    }
    
    buttons.emplace_back(new Button(pin, longPressDuration));
    buttons.back()->initComponent(serialDebug);
    
    buttons.back()->addListener(std::bind(&ButtonManager::gotButtonEvent, this, std::placeholders::_1));
    compDebug("button registered on pin "+String(pin));
}

void ButtonManager::update()
{
    if (!checkInit())
        return;

    for (auto const &button : buttons) button->update();
}


void ButtonManager::gotButtonEvent(const ButtonEvent &e)
{
    compDebug("got button " + String(e.type));
    sendEvent(e);
}