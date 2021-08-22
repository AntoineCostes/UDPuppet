#pragma once
#include "../../Config.h"

#include <Arduino.h>
#include <Preferences.h>
#include <OSCMessage.h>

#include <vector>
#include <map>
#include <set>
#include <memory>
#include <string.h>
#include <iterator>

// Component is an abstract class for each manager, allowing for
// - dynamic parameter storage in flash memory (to remember value after reboot)
// - custom log to help debugging
// - OSC command handling 
// - pin usage handling
class Component
{
public:
    Component(const String &name);
    //virtual ~Component() {}

    String name; // TODO const ?

    virtual void initComponent(bool serialDebug);
    virtual void update() = 0;

    virtual bool handleCommand(OSCMessage &command);

    static std::set<int> forbiddenPins;

    bool checkInit();

protected:
    bool serialDebug;

    // parameters
    std::map<String, String> stringParameters;
    std::map<String, int> intParameters;
    std::map<String, float> floatParameters;
    std::map<String, bool> boolParameters;

    // Log helpers
    void compDebug(String message);
    void compLog(String message);
    void compError(String message);

    // parameter storage
    void clearFlash();
    void overrideFlashParameters();
    void readParamsFromFlash();

    // command handling
    bool checkCommandArguments(OSCMessage &command, String types, bool logError);

private:
    bool initialized;
};


struct var
{
    char type;
    union
    {
        int i;
        float f;
        char * s;
    } value;

    int intValue() const
    {
        switch (type)
        {
        case 'i':
            return value.i;
        case 'f':
            return (int)value.f;
        case 's':
            return String(value.s).toInt();
        }
        return 0;
    }

    float floatValue() const
    {
        switch (type)
        {
        case 'i':
            return (float)value.i;
        case 'f':
            return value.f;
        case 's':
            return String(value.s).toFloat();
        }

        return 0;
    }

    String stringValue() const
    {
        switch (type)
        {
        case 'i':
            return String(value.i);
        case 'f':
            return String(value.f);
        case 's':
            return value.s;
        }
        return "";
    }
};
