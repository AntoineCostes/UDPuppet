#include "BatteryManager.h"

#define HIGH_BATTERY 2100 
// #define HIGH_BATTERY 2300 // ~4.15V
#define LOW_BATTERY 1980  
// #define LOW_BATTERY 1930  // ~3.5V

#ifdef ESP32
BatteryManager::BatteryManager() : Manager("battery"), smoothing(.9f), sensorAvailable(false)
{
    serialDebug = BATTERY_DEBUG;
}

void BatteryManager::initManager()
{
    Manager::initManager();
    lastPingMs = 0;
    lastMeasureMs = 0;
    
    switch (BOARD_TYPE)
    {
    case HUZZAH32:
        analogSetPinAttenuation(A13, ADC_11db);
        break;

    case HUZZAH32_S3:
        compLog("Check lipo sensor");
        if (!lipoSensor.begin()) {
            compError("Lipo sensor MAX17048 not found, make sure a battery is plugged in!");
        } else sensorAvailable = true;
        break;

    default:
        compError("Unknown board type !");
        return;
    }
}

void BatteryManager::update()
{
    switch (BOARD_TYPE)
    {
        case HUZZAH32:
            // smooth measurement at 10Hz
            if (millis() > lastMeasureMs + BATTERY_WINDOW_MS)
            {
                smoothedValue = ((1.0f-smoothing) * analogRead(A13)) + smoothing*smoothedValue;
                lastMeasureMs = millis();
            }

            // send level and voltage based on smoothed value
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
            break;

        case HUZZAH32_S3:
            if (sensorAvailable)
            {
                // send level and voltage based on smoothed value
                if (millis() > lastPingMs + BATTERY_TIMEOUT_MS)
                {
                    voltage = lipoSensor.cellVoltage();
                    level = lipoSensor.cellPercent();

                    compDebug(String(smoothedValue) + "\t voltage = " + String(voltage) + "V\t " + level + "%");

                    if (voltage < BATTERY_LOW_VOLTAGE) 
                        sendEvent(BatteryEvent(BatteryEvent::Type::BATTERY_LOW, level, voltage, int(smoothedValue)));
                    else
                        sendEvent(BatteryEvent(BatteryEvent::Type::PING, level, voltage, int(smoothedValue)));

                    lastPingMs = millis();
                }
            }
            break;
    }
}
#endif