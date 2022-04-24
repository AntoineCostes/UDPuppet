#include "BatteryManager.h"

#ifdef ESP32
BatteryManager::BatteryManager() : Manager("battery")
{
    analogSetPinAttenuation(A13, ADC_0db);
    serialDebug = MASTER_DEBUG;
}

void BatteryManager::initManager()
{
    Manager::initManager();
    lastPingMs = 0;
}

void BatteryManager::update()
{
    if (millis() > lastPingMs + BATTERY_TIMOUT_MS)
    {
        analogValue = analogRead(A13);
        voltage = (2*analogValue / 4096.0f)*3.3f;
        normValue = voltage / 4.2f;

        sendEvent(BatteryEvent(BatteryEvent::Type::PING, normValue, voltage, analogValue));
        lastPingMs = millis();
    }
}
#endif