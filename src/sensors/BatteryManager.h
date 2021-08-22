#include "../common/Manager.h"
#include "../utils/EventBroadcaster.h"

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
    float normValue;
    float voltage;
    int analogValue;
    BatteryEvent(Type type, float normValue, float voltage, int analogValue) : type(type), normValue(normValue), voltage(voltage), analogValue(analogValue) {}
};

class BatteryManager : public Manager,
                       public EventBroadcaster<BatteryEvent>
{
public:
    BatteryManager();

    void initManager();
    void update();

protected:
    long lastPingMs;
    int analogValue;
    float normValue;
    float voltage;
};