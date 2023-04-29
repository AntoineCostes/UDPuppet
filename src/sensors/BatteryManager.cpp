#include "BatteryManager.h"

#ifdef ESP32
BatteryManager::BatteryManager() : Manager("battery"), smoothing(.9f)
{
    analogSetPinAttenuation(A13, ADC_11db);
    serialDebug = MASTER_DEBUG;
}

void BatteryManager::initManager()
{
    Manager::initManager();
    lastPingMs = 0;
    lastMeasureMs = 0;
}

void BatteryManager::update()
{
    if (millis() > lastMeasureMs + BATTERY_WINDOW_MS)
    {
        smoothedValue = ((1.0f-smoothing) * analogRead(A13)) + smoothing*smoothedValue;
        lastMeasureMs = millis();
    }

    if (millis() > lastPingMs + BATTERY_TIMEOUT_MS)
    {
        // estimate voltage and level 
        voltage = (2*smoothedValue / 4095.0f)*3.3f*1.123f;
        level = min(1.0f, max(0.0f, (voltage - 3.5f)/(4.2f - 3.5f))); // [0-1] from 3.5V to 4.2V

        if (voltage < BATTERY_LOW_VOLTAGE) 
            sendEvent(BatteryEvent(BatteryEvent::Type::BATTERY_LOW, level, voltage, int(smoothedValue)));
        else
            sendEvent(BatteryEvent(BatteryEvent::Type::PING, level, voltage, int(smoothedValue)));

        lastPingMs = millis();
    }
}
#endif