#pragma once
#include "Component.h"
using std::vector;

// Manager is an abstract class to handle a given type of hardware (leds, motors, etc)
// It has a variable list of Components, and keeps track of used pins
class Manager : public Component
{
public:
    Manager(const String &name);

    void initManager();
    void update() override;

protected:
    bool serialDebug;
    
    vector<std::unique_ptr<Component>> props;
    bool registerProp(Component* comp, std::set<byte> reservedPins);
};