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
    compDebug("registering prop: " + String(prop->name));

    for (int newPin : reservedPins)
        if (Component::forbiddenPins.find(newPin) != Component::forbiddenPins.end())
        {
            compError("cannot register prop: " + String(newPin) + " is already used !"); 
            return false;
        }

    Component::forbiddenPins.insert(reservedPins.begin(), reservedPins.end());
    props.emplace_back(prop);
    prop->initComponent(serialDebug);

    return true;
}
    
void Manager::update()
{
    if (!checkInit())
        return;
        
    for (auto const &prop : props)
    {
        prop.get()->update();
    }
}


