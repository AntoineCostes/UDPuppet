#include "LedManager.h"

// TODO parameter led refresh time
LedManager::LedManager() : Manager("led")
{
    serialDebug = LED_DEBUG;
}

void LedManager::initManager()
{
    Manager::initManager();
}

void LedManager::registerLedStrip(int pin, int numLeds, neoPixelType type, bool wifiDebug, bool useInSequences)
{
    if (!checkInit())
        return;

    compDebug("registering led strip on pin " + String(pin));

    if (!Component::registerPin(pin))
    {
        compError("cannot register led strip !");
        return;
    }

    strips.emplace_back(new LedStrip(pin, numLeds, type, wifiDebug, useInSequences));
    strips.back()->initComponent(serialDebug);
    compLog("registered led strip: " + String(strips.back()->name));
}

void LedManager::update()
{
    if (!checkInit())
        return;

    float slow = abs(sin(0.001f * millis()));
    float fast = abs(sin(0.002f * millis()));

    // notify
    for (auto const &strip : strips) 
    {
        if (strip->isNotifying)
        {
            switch (currentNotification)
            {
            case Notification::READY:
                strip->setAll(100 * notificationLight, 100 * notificationLight, 100 * notificationLight);
                notificationLight *= 0.999f;
                break;

            case Notification::ERROR:
                strip->setAll(50, 0, 0);
                break;

            case Notification::WORKING:
                strip->setAll(int(50 * fast), 0, int(50 * fast));
                break;

            case Notification::WAITING:
                strip->setAll(0, 0, int(50 * slow));
                break;
                
//         setAll(int(250 * slow), int(250 * slow), int(250 * slow));
            }
        }
        strip->update();
    }
    
    Manager::update(); // led strips will refresh periodically
}

void LedManager::clear()
{
    for (auto const &strip : strips) strip->clear();
}

void LedManager::setColor(int c)
{
    setColor(c, c, c);
}

void LedManager::setColor(int r, int g, int b)
{
    for (auto const &strip : strips) 
    {
        strip->isNotifying = false;
        strip->setAll(r, g, b);
    }
}

void LedManager::setBrightness(float value)
{
    for (auto const &strip : strips) strip->setBrightness(value);
}

void LedManager::setBrightness(int stripIndex, float value)
{
    strips[stripIndex]->setBrightness(value);
}

void LedManager::setColor(int stripIndex, int c)
{
    strips[stripIndex]->isNotifying = false;
    setColor(stripIndex, c, c, c);
}

void LedManager::setColor(int stripIndex, int r, int g, int b)
{
    strips[stripIndex]->isNotifying = false;
    strips[stripIndex]->setAll(r, g, b);
}

void LedManager::setColor(int stripIndex, int i, int r, int g, int b)
{
    strips[stripIndex]->isNotifying = false;
    strips[stripIndex]->setLed(i, r, g, b);
}

void LedManager::setWifiDebug(int stripIndex, bool value)
{
    strips[stripIndex]->setWifiDebug(value);
}

bool LedManager::useInSequences(int stripIndex)
{
    return strips[stripIndex]->useInSequences;
}

void LedManager::notify(Notification notification)
{
    currentNotification = notification;

    for (auto const &strip : strips)  
        if (strip->isWifiDebug())
            strip->isNotifying = true;
    
    switch (currentNotification)
    {
        case Notification::READY:
            compDebug("========= READY");
            notificationLight = 1.0f;
            break;

        case Notification::ERROR:
            compDebug("========= ERROR");
            break;

        case Notification::WORKING:
            compDebug("========= WORKING");
            break;

        case Notification::WAITING:
            compDebug("========= WAITING");
            break;
    }
}

// void LedManager::forceNotify(int stripIndex, Notification notification)
// {
//     currentNotification = notification;
    
//     strips[stripIndex]->isNotifying = true;
// }

bool LedManager::handleCommand(OSCMessage &command)
{
    if (!checkInit())
        return false;

    char buf[32];
    command.getAddress(buf);
    String address = String(buf);
    compLog("handle command : " + address);

    if (address.equals("/led/debug"))
    {
        if (checkCommandArguments(command, "i", false))
        {
            int value = command.getInt(0)>0;
            strips[0]->setWifiDebug(value);
            return true;
        } else if (checkCommandArguments(command, "ii", true))
        {
            int index = command.getInt(0);
            int value = command.getInt(1)>0;
            strips[index]->setWifiDebug(value);
            return true;
        }
    }
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
            return true;
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
    return false;
}