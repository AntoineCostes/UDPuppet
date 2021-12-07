#include "SensorManager.h"

SensorManager::SensorManager() : Manager("sensors"), batteryPin(-1)
{
    switch (BOARD_TYPE)
    {
        case HUZZAH32:
            batteryPin = A13;
            break;
    }

    if (batteryPin) analogSetPinAttenuation(batteryPin, ADC_0db);

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

            sendEvent(BatteryEvent(BatteryEvent::Type::PING, batteryRelativeValue, batteryVoltageValue, batteryAnalogValue));
            lastBatteryPingMs = millis();
        }
        
    for (auto const &analog : analogs)
    {   
        int raw = analog->read();   

        if (raw >= 0) // if new value
        {   
            float norm = float(raw) / 1024.0f;
            sendEvent(AnalogEvent(analog->niceName, raw, norm));
        }
    }
}