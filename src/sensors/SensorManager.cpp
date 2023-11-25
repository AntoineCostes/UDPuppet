#include "SensorManager.h"

SensorManager::SensorManager() : Manager("sensor")
{
}

void SensorManager::initManager()
{
    Manager::initManager();
}

void SensorManager::registerHCSR04(String niceName, byte triggerPin, byte echoPin, boolean active)
{    
    std::set<int> pins = {triggerPin, echoPin};
    if (!Component::registerPins(pins))
    {
        compError("cannot register sensor reader: a pin is already registered !");
        return;
    }

    ultrasonics.emplace_back(new HCSR04Reader(triggerPin, echoPin, niceName, active));
    ultrasonics.back()->initComponent(serialDebug);
    compDebug("register prop: " + niceName);
}


void SensorManager::update()
{
    for (auto const &sensor : ultrasonics)
    {   
        sensor->update();

        if (sensor->gotNewValue)
        {
            sendEvent(SensorValueEvent(sensor->niceName, sensor->distanceValueMm, sensor->normValue));
        }
    }
}

bool SensorManager::handleCommand(OSCMessage &command)
{
    if (!checkInit())
        return false;

    char buf[32];
    command.getAddress(buf);
    String address = String(buf);
    compLog("handle command: " + address);

    if (address.equals("/ultrasonics/sensor"))
    {
        if (checkCommandArguments(command, "ii", true))
        {
            byte index = command.getInt(0);
            bool value = command.getInt(1) > 0;

            if (index >= 0 && index < ultrasonics.size())
            {
                 compLog("set sensor ");
                ultrasonics[index]->active = value;
                return true;
            }
        }
    }
    return false;
}