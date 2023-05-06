#pragma once
#include "../../Config.h"

#include <Arduino.h>
#ifdef ESP32
#include <Preferences.h>
#endif
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
    bool useInSequences; // TODO put in constructor

    virtual void initComponent(bool serialDebug);
    bool checkInit(); // TODO remove ?
    virtual void update() = 0; // TODO make not pure virtual ?
 
    static bool registerPin(int pin);
    static bool registerPins(std::set<int> pins);
    static std::set<int> forbiddenPins;

    virtual bool handleCommand(OSCMessage &command);

protected:
    bool initialized;
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
};

