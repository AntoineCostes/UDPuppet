#pragma once
#include "Component.h"
using std::vector;

// Manager is an abstract class to handle a given type of hardware (leds, motors, etc)
// It has a variable list of Components, and keeps track of used pins
class Manager : public Component
{
public:
    Manager(const String &name);

    virtual void initManager();
    virtual void update() override;

    // Log helpers
    void dbg(String message);
    void log(String message);
    void err(String message);

protected:
    vector<std::unique_ptr<Component>> props;
    bool registerProp(Component* comp, std::set<int> reservedPins);
};