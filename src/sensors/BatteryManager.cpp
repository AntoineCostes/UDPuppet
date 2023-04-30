#include "BatteryManager.h"

#define HIGH_BATTERY 2300 // ~4.15V
#define LOW_BATTERY 1930  // ~3.5V

#ifdef ESP32
BatteryManager::BatteryManager() : Manager("battery"), smoothing(.9f)
{
    analogSetPinAttenuation(A13, ADC_11db);
    serialDebug = BATTERY_DEBUG;
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
        // estimate voltage
        voltage = (2*smoothedValue / 4095.0f)*3.3f*1.123f;

        // estimate percetage from 3.5V to 4.2V
        level = min(100, max(0, int(map(int(smoothedValue), LOW_BATTERY, HIGH_BATTERY, 0, 100)))); 

        compDebug(String(smoothedValue) + "\t voltage = " + String(voltage) + "V\t " + level + "%");

        if (voltage < BATTERY_LOW_VOLTAGE) 
            sendEvent(BatteryEvent(BatteryEvent::Type::BATTERY_LOW, level, voltage, int(smoothedValue)));
        else
            sendEvent(BatteryEvent(BatteryEvent::Type::PING, level, voltage, int(smoothedValue)));

        lastPingMs = millis();
    }
}
#endif