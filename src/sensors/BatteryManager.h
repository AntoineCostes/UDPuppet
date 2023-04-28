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
    int values[BATTERY_BUFFER_SIZE];
    int increment;
    int lastMeasureMs;

    long lastPingMs;
    int analogValue;
    int smoothedValue;
    float normValue;
    float voltage;
};
#endif
