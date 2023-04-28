#include "BatteryManager.h"

#ifdef ESP32
BatteryManager::BatteryManager() : Manager("battery"), increment(0)
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
        values[increment] = analogRead(A13);
        increment = (increment + 1) % BATTERY_BUFFER_SIZE;
        lastMeasureMs = millis();
    }

    if (millis() > lastPingMs + BATTERY_TIMEOUT_MS)
    {
        int smoothedValue;
        for (int i = 0; i < BATTERY_BUFFER_SIZE; i++) smoothedValue += values[i];
        smoothedValue /= BATTERY_BUFFER_SIZE;

        voltage = (2*smoothedValue / 4095.0f)*3.3f*1.123f;
        normValue = min(1.0f, max(0.0f, (voltage - 3.5f)/(4.2f - 3.5f))); // [0-1] from 3.5V to 4.2V

        sendEvent(BatteryEvent(BatteryEvent::Type::PING, normValue, voltage, smoothedValue));
        lastPingMs = millis();
    }
}
#endif