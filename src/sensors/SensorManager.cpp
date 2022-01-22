#include "SensorManager.h"

SensorManager::SensorManager() : Manager("sensors"), batteryPin(-1)
{
    #ifdef ESP32
    switch (BOARD_TYPE)
    {
        case HUZZAH32:
            batteryPin = A13;
            break;
    }

    if (batteryPin) analogSetPinAttenuation(batteryPin, ADC_0db);
    #endif
    serialDebug = MASTER_DEBUG;
}

void SensorManager::registerAnalogReader(String niceName, byte pin)
{    
    if (!Component::registerPin(pin))
    {
        compError("cannot register analog reader: a pin is already registered !");
        return;
    }

    analogs.emplace_back(new AnalogReader(pin, niceName));
    analogs.back()->initComponent(serialDebug);
    compDebug("register prop: " + niceName);
}

void SensorManager::registerHCSR04Reader(String niceName, byte triggerPin, byte echoPin, boolean active)
{    
    if (!Component::registerPins(std::set<byte>{triggerPin, echoPin}))
    {
        compError("cannot register hcsr04 reader: a pin is already registered !");
        return;
    }

    ultrasonics.emplace_back(new HCSR04Reader(triggerPin, echoPin, niceName, active));
    ultrasonics.back()->initComponent(serialDebug);
    compDebug("register prop: " + niceName);
}

void SensorManager::initManager()
{
    Manager::initManager();
    lastBatteryPingMs = 0;
}

void SensorManager::update()
{
    if (batteryPin)
        if (millis() > lastBatteryPingMs + BATTERY_TIMOUT_MS)
        {
            batteryAnalogValue = analogRead(batteryPin);
            batteryVoltageValue = (2*batteryAnalogValue / 4096.0f)*3.3f;
            batteryRelativeValue = batteryVoltageValue / 4.2f;

            sendEvent(SensorValueEvent("battery", batteryAnalogValue, 2048));
            sendEvent(SensorValueEvent("batteryVoltage", batteryVoltageValue, batteryRelativeValue));
            lastBatteryPingMs = millis();
        }
        
    for (auto const &analog : analogs)
    {   
        int raw = analog->read();   

        if (raw >= 0) // if new value
        {   
            float norm = float(raw) / 1024.0f;
            sendEvent(SensorValueEvent(analog->niceName, raw, norm));
        }
    }
    
    for (auto const &hcsr04 : ultrasonics)
    {   
        hcsr04->update();

        if (hcsr04->gotNewValue)
        {
            sendEvent(SensorValueEvent(hcsr04->niceName, hcsr04->distanceValueMm, hcsr04->normValue));
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

#ifdef NUM_HCSR04
    if (address.equals("/sensors/hcsr04"))
    {
        if (checkCommandArguments(command, "ii", true))
        {
            byte index = command.getInt(0);
            bool value = command.getInt(1) > 0;

            // FIXME
            if (index >= 0 && index < NUM_HCSR04)
            {
                 compLog("set hcsr04 ");
                ultrasonics[index]->active = value;
                return true;
            }
        }
    }
    #endif
    return false;
}