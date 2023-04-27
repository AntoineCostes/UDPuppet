#include "BatteryManager.h"

#ifdef ESP32
BatteryManager::BatteryManager() : Manager("battery")
{
    analogSetPinAttenuation(A13, ADC_11db);
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
        voltage = (2*analogValue / 4095.0f)*3.3f*1.123f;
        normValue = min(1.0f, max(0.0f, (voltage - 3.5f)/(4.2f - 3.5f))); // [0-1] from 3.5V to 4.2V

        sendEvent(BatteryEvent(BatteryEvent::Type::PING, normValue, voltage, analogValue));
        lastPingMs = millis();
    }
}
#endif