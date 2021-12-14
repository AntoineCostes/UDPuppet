#include "../common/Manager.h"
#include "../utils/EventBroadcaster.h"
#include "AnalogReader.h"
#include "HCSR04Reader.h"

class BatteryEvent
{
public:
    // enum Type
    // {
    //     PING,
    //     BATTERY_FULL,
    //     BATTERY_LOW, 
    //     BATTERY_DEAD
    // } type;
    float normValue;
    float voltage;
    int analogValue;
    BatteryEvent(/*Type type, */float normValue, float voltage, int analogValue) : /*type(type),*/ normValue(normValue), voltage(voltage), analogValue(analogValue) {}
};

class AnalogEvent
{
public:
    String niceName;
    int rawValue;
    float normValue;
    AnalogEvent(String niceName, int rawValue, float normValue) : niceName(niceName), rawValue(rawValue), normValue(normValue) {}
};

class SensorManager : public Manager,
                        public EventBroadcaster<BatteryEvent>,
                        public EventBroadcaster<AnalogEvent>
{
public:
    SensorManager();
    using EventBroadcaster<BatteryEvent>::addListener;
    using EventBroadcaster<BatteryEvent>::sendEvent;
    using EventBroadcaster<AnalogEvent>::addListener;
    using EventBroadcaster<AnalogEvent>::sendEvent;

    void initManager();
    void update();

    void registerAnalogReader(String niceName, byte pin);
    void registerHCSR04Reader(String niceName, byte triggerPin, byte echoPin);

protected:
    std::vector<AnalogReader*> analogs; 
    std::vector<HCSR04Reader*> ultrasonics; 

    // battery
    byte batteryPin;
    long lastBatteryPingMs;
    int batteryAnalogValue;
    float batteryVoltageValue;
    float batteryRelativeValue;
};