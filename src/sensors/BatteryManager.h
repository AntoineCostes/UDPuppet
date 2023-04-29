#include "../common/Manager.h"
#include "../utils/EventBroadcaster.h"

#ifdef ESP32
class BatteryEvent
{
public:
    enum Type
    {
        PING,
        BATTERY_FULL,
        BATTERY_LOW,
        BATTERY_DEAD
    } type;
    float level;
    float voltage;
    int analogValue;
    BatteryEvent(Type type, float level, float voltage, int analogValue) : type(type), level(level), voltage(voltage), analogValue(analogValue) {}
};

class BatteryManager : public Manager,
                       public EventBroadcaster<BatteryEvent>
{
public:
    BatteryManager();

    void initManager();
    void update();

protected:
    float smoothedValue;
    float smoothing;
    int lastMeasureMs;

    float voltage;
    float level;
    long lastPingMs;
};
#endif
