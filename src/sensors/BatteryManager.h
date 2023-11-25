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
    int level;
    float voltage;
    int analogValue;
    BatteryEvent(Type type, int level, float voltage, int analogValue) : type(type), level(level), voltage(voltage), analogValue(analogValue) {}
};

class BatteryManager : public Manager,
                       public EventBroadcaster<BatteryEvent>
{
public:
    BatteryManager();

    void initManager();
    void update();

protected:
    // TODO make child class
    Adafruit_MAX17048 lipoSensor;
    bool sensorAvailable;

    float smoothedValue;
    float smoothing;
    int lastMeasureMs;

    float voltage;
    int level;
    long lastPingMs;
};
#endif
