#include "LedManager.h"

#ifdef HAS_LED
LedManager::LedManager() : Manager("led"),
                           mode(LedMode::WAITING),
                           toastTimer(0)
{
    boolParameters["debug"] = true;
}

void LedManager::initManager()
{
    Manager::initManager();
    lastLedRefreshTime = millis();
    serialDebug = LED_DEBUG;
}

void LedManager::registerLedStrip(byte index, byte pin, byte numLeds, neoPixelType type)
{
    if (!checkInit())
        return;

    // std::set<int> pins {pin};

    if (Component::forbiddenPins.find(pin) != Component::forbiddenPins.end())
    {
        compError("cannot register prop: " + String(pin) + " is already used !");
        return;
    }
    Component::forbiddenPins.insert(pin);

    strips.insert({index, new LedStrip(pin, numLeds, type)});
    strips[index]->initComponent(serialDebug);
    compDebug("register prop: " + String(strips[index]->name));
}

void LedManager::setMode(LedMode newMode)
{
    //compDebug("set mode");
    //if (newMove != mode)
    //    setColor(0, 0, 0);
    //mode = newMode;
}

void LedManager::toast(LedMode toastedMode, long ms)
{
    toastMode = toastedMode;

    toastTimer.time = ms;

    toastTimer.start();
}

void LedManager::update()
{
    if (!checkInit())
        return;

    float slow = abs(sin(0.001f * millis()));
    float fast = abs(sin(0.002f * millis()));

    toastTimer.update();
    LedMode currentMode = toastTimer.isRunning ? toastMode : mode;

    // clear leds
    //if (!toastTimer.isRunning && toastMode != mode)
    //{
    //    compDebug("end toast");
    //    toastMode = mode;
    //    setColor(0, 0, 0);
    //}

    if (boolParameters["debug"])
        switch (currentMode)
        {
        case LedMode::WAITING:
            setColor(0, 0, int(50 * slow));
            break;

        case LedMode::READY:
            setColor(0, 100, 0);
            break;

        case LedMode::WORKING:
            setColor(int(50 * fast), 0, int(50 * fast));
            break;

        case LedMode::ERROR:
            setColor(50, 0, 0);
            break;

        case LedMode::STREAMING:
            break;
        }

    // no Manager::update(), leds are refreshed periodically
    if (millis() - lastLedRefreshTime > LED_REFRESH_MS)
    {
        for (auto const &pair : strips)
        {
            pair.second->update();
        }
        lastLedRefreshTime = millis();
    }
}

void LedManager::clear()
{
    for (auto const &pair : strips)
        pair.second->clear();
}

void LedManager::setColor(int c)
{
    setColor(c, c, c);
}

void LedManager::setColor(int r, int g, int b)
{
    for (auto const &pair : strips)
        pair.second->setAll(r, g, b);
}

void LedManager::setBrightness(float value)
{
    for (auto const &pair : strips)
        pair.second->setBrightness(value);
}

void LedManager::setBrightness(int stripIndex, float value)
{
    strips[stripIndex]->setBrightness(value);
}

void LedManager::setColor(int stripIndex, int c)
{
    strips[stripIndex]->setAll(c, c, c);
}

void LedManager::setColor(int stripIndex, int r, int g, int b)
{
    strips[stripIndex]->setAll(r, g, b);
}

void LedManager::setColor(int stripIndex, int i, int r, int g, int b)
{
    strips[stripIndex]->setLed(i, r, g, b);
}

void LedManager::setDebug(bool value)
{
    setColor(0, 0, 0);
    boolParameters["debug"] = value;
    overrideFlashParameters();
}
bool LedManager::handleCommand(OSCMessage &command)
{
    if (!checkInit())
        return false;

    char buf[32];
    command.getAddress(buf);
    String address = String(buf);
    compLog("handle command : " + address);

    if (address.equals("/led/color"))
    {
        if (checkCommandArguments(command, "iii", false))
        {
            int r = command.getInt(0);
            int g = command.getInt(1);
            int b = command.getInt(2);
            setColor(r, g, b);
            return true;
        } else if (checkCommandArguments(command, "iiii", false))
        {
            int stripIndex = command.getInt(0);
            int r = command.getInt(1);
            int g = command.getInt(2);
            int b = command.getInt(3);
            setColor(stripIndex, r, g, b);
            return true;
        } else if (checkCommandArguments(command, "iiiii", false))
        {
            int stripIndex = command.getInt(0);
            int ledIndex = command.getInt(1);
            int r = command.getInt(2);
            int g = command.getInt(3);
            int b = command.getInt(4);
            setColor(stripIndex, ledIndex, r, g, b);
            return true;
        } else if (checkCommandArguments(command, "fff", false))
        {
            float r = command.getFloat(0);
            float g = command.getFloat(1);
            float b = command.getFloat(2);
            setColor(r * 255, g * 255, b * 255);
        } else if (checkCommandArguments(command, "ifff", true))
        {
            int stripIndex = command.getInt(0);
            float r = command.getFloat(1);
            float g = command.getFloat(2);
            float b = command.getFloat(3);
            setColor(stripIndex, r * 255, g * 255, b * 255);
            return true;
        }
    }
    else if (address.equals("/led/brightness"))
    {
        if (checkCommandArguments(command, "f", false))
        {
            float b = command.getFloat(0);
            setBrightness(b);
            return true;
        } else if (checkCommandArguments(command, "if", true))
        {
            int index = command.getInt(0);
            float b = command.getFloat(1);
            setBrightness(index, b);
            return true;
        }
    }
    else if (address.equals("/led/debug"))
    {
        if (checkCommandArguments(command, "i", false))
        {
            int value = command.getFloat(0);
            setDebug(value>0);
            return true;
        }
    }
    return false;
}
#endif