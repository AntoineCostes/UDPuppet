#include "Component.h"

std::set<byte> Component::forbiddenPins = {};

Component::Component(const String &name) : name(name), initialized(false)
{
}

void Component::initComponent(bool debug)
{
    serialDebug = debug;
    readParamsFromFlash();
    initialized = true;
}

bool Component::checkInit()
{
    if (!initialized)
        compError("component not initialized !");
    return initialized;
}

bool Component::checkRange(String valueName, int value, int min, int max)
{
  if (value < min || value > max)
  {
    compError("can't set " + valueName + ", value " + String(value) + "us out of range [" + String(min) + ", " + String(max) + "].");
    return false;
  }
  return true;
}

bool Component::checkRange(String valueName, float value, float min, float max)
{
  if (value < min || value > max)
  {
    compError("can't set " + valueName + ", value " + String(value) + "us out of range [" + String(min) + ", " + String(max) + "].");
    return false;
  }
  return true;
}

bool Component::registerPin(byte pin)
{
    std::set<byte> pins = {pin};
    return registerPins(pins);
}

bool Component::registerPins(std::set<byte> pins)
{
    for (byte pin : pins)
    {
        if (Component::forbiddenPins.find(pin) != Component::forbiddenPins.end())
        {
            Serial.println("[Component] Error: pin #"+String(pin)+" is already registered !");
            return false;
        }
    }
    for (byte pin : pins)
    {
        Component::forbiddenPins.insert(pin);
    }
    return true;
}

void Component::compDebug(String message)
{
    if (serialDebug)
        Serial.println("[" + name + "] " + message);
}

void Component::compLog(String message)
{
    Serial.println("[" + name + "] " + message);
}

void Component::compError(String message)
{
    Serial.println("[" + name + " ERROR] " + message);
}

void Component::clearFlash()
{
    Preferences preferences;
    compLog("clear flash memory");
    preferences.begin(name.c_str());
    preferences.clear();
    preferences.end();
}

void Component::overrideFlashParameters()
{
    Preferences preferences;
    compDebug("--- override parameters in flash memory");
    preferences.begin(name.c_str());

    for (auto const &x : stringParameters)
    {
        String paramName = x.first;
        String value = x.second;

        if (preferences.isKey(paramName.c_str()))
            compDebug(paramName + " string param was stored as " + preferences.getString(paramName.c_str()));
        else
            compDebug(paramName + " was not stored yet");

        preferences.putString(paramName.c_str(), value.c_str());
        compDebug(paramName + " is now " + value);
    }

    for (auto const &i : intParameters)
    {
        String paramName = i.first;
        int value = i.second;

        if (preferences.isKey(paramName.c_str()))
            compDebug(paramName + " int param was stored as " + String(preferences.getInt(paramName.c_str())));
        else
            compDebug(paramName + " was not stored yet");

        preferences.putInt(paramName.c_str(), value);
        compDebug(paramName + " is now " + value);
    }

    for (auto const &f : floatParameters)
    {
        String paramName = f.first;
        float value = f.second;

        if (preferences.isKey(paramName.c_str()))
            compDebug(paramName + " float param was stored as " + String(preferences.getFloat(paramName.c_str())));
        else
            compDebug(paramName + " was not stored yet");

        preferences.putFloat(paramName.c_str(), value);
        compDebug(paramName + " is now " + value);
    }

    for (auto const &b : boolParameters)
    {
        String paramName = b.first;
        bool value = b.second;

        if (preferences.isKey(paramName.c_str()))
            compDebug(paramName + " bool param was stored as " + String(preferences.getBool(paramName.c_str())));
        else
            compDebug(paramName + " was not stored yet");

        preferences.putBool(paramName.c_str(), value);
        compDebug(paramName + " is now " + value);
    }

    preferences.end();
    compDebug("---");
}

void Component::readParamsFromFlash()
{
    Preferences preferences;
    compDebug("--- retrieve parameters from flash memory");
    preferences.begin(name.c_str());

    // for each parameter, according to its type, override its value if it is already stored in flash memory
    for (auto const &x : stringParameters)
    {
        String paramName = x.first;
        String value = x.second;

        if (preferences.isKey(paramName.c_str()))
        {
            compDebug("set " + paramName + " to " + preferences.getString(paramName.c_str()) + " (default is " + value + ")");
            stringParameters[paramName] = preferences.getString(paramName.c_str(), value.c_str());
        }
        else
        {
            compDebug(paramName + " was not stored yet, default is " + value);
            preferences.putString(paramName.c_str(), value.c_str());
        }
    }
    for (auto const &i : intParameters)
    {
        String paramName = i.first;
        int value = i.second;

        if (preferences.isKey(paramName.c_str()))
        {
            compDebug("set " + paramName + " to " + preferences.getInt(paramName.c_str()) + " (default is " + String(value) + ")");
            intParameters[paramName] = preferences.getInt(paramName.c_str(), value);
        }
        else
        {
            compDebug(paramName + " was not stored yet, default is " + String(value));
            preferences.putInt(paramName.c_str(), value);
        }
    }
    for (auto const &f : floatParameters)
    {
        String paramName = f.first;
        float value = f.second;

        if (preferences.isKey(paramName.c_str()))
        {
            compDebug("set " + paramName + " to " + preferences.getFloat(paramName.c_str()) + " (default is " + String(value) + ")");
            floatParameters[paramName] = preferences.getFloat(paramName.c_str(), value);
        }
        else
        {
            compDebug(paramName + " was not stored yet, default is " + String(value));
            preferences.putFloat(paramName.c_str(), value);
        }
    }
    for (auto const &b : boolParameters)
    {
        String paramName = b.first;
        bool value = b.second;

        if (preferences.isKey(paramName.c_str()))
        {
            compDebug("set " + paramName + " to " + preferences.getBool(paramName.c_str()) + " (default is " + String(value) + ")");
            boolParameters[paramName] = preferences.getBool(paramName.c_str(), value);
        }
        else
        {
            compDebug(paramName + " was not stored yet, default is " + String(value));
            preferences.putBool(paramName.c_str(), value);
        }
    }
    preferences.end();
    compDebug("---");
}

bool Component::checkCommandArguments(OSCMessage &command, String types, bool logError)
{
    if (command.hasError())
    {
        compError("command has error");
        return false;
    }

    // check number of arguments
    if (command.size() != types.length())
    {
        if (logError)
            compError("typetag should be " + types);
        return false;
    }

    // check types
    for (int i = 0; i < command.size(); i++)
        if (command.getType(i) != types.charAt(i))
        {
            if (logError)
                compError("typetag should be " + types);
            return false;
        }

    return true;
}

bool Component::handleCommand(OSCMessage &command)
{
    return false;
}