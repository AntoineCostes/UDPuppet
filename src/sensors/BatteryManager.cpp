#include "BatteryManager.h"

BatteryManager::BatteryManager() : Manager("battery")
{
    int batteryPin = -1;
    switch (BOARD_TYPE)
    {
        case HUZZAH32:
            batteryPin = A13;
            break;
    }

    if (batteryPin) analogSetPinAttenuation(batteryPin, ADC_0db);

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