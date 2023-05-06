#include "Manager.h"

// TODO: remove props ? chaque manager a sa liste typée
// faire une Interface ?
Manager::Manager(const String &name) : Component(name)
{
}

void Manager::initManager()
{
    Component::initComponent(serialDebug);
}

bool Manager::registerProp(Component* prop, std::set<int> reservedPins)
{
    compDebug("register prop: " + String(prop->name));

    if (!Component::registerPins(reservedPins))
    {
        compError("failed !");
        return false;
    }

    props.emplace_back(prop);
    prop->initComponent(serialDebug);

    return true;
}
    
void Manager::update()
{
    for (auto const &prop : props)
    {
        prop.get()->update();
    }
}

void Manager::dbg(String message)
{
    if (serialDebug)
        Serial.println("[" + name + "] " + message);
}

void Manager::log(String message)
{
    Serial.println("[" + name + "] " + message);
}

void Manager::err(String message)
{
    Serial.println("[" + name + " ERROR] " + message);
}

