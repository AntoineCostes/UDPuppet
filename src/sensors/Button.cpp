#include "Button.h"

Button::Button(int pin) : Component("button" + String(pin)), pin(pin), button(pin)
{
}

void Button::initComponent(bool serialDebug)
{
    button.begin();
    Component::initComponent(serialDebug);

    // button.onPressed(Button::pressed);   
}

void Button::update()
{
    if (!checkInit())
        return;

    button.read();
}

void Button::pressed()
{
    sendEvent(ButtonEvent(pin, ButtonEvent::Type::PRESSED));
}