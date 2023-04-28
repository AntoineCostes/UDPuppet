#include "../common/Manager.h"
#include "../utils/EventBroadcaster.h"

#define BATTERY_BUFFER_SIZE 50

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
    int values[BATTERY_BUFFER_SIZE];
    int increment;
    int lastMeasureMs;

    long lastPingMs;
    int analogValue;
    int smoothedValue;
    float level;
    float voltage;
};
#endif
