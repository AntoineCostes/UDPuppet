#include "../common/Manager.h"
#include "../utils/EventBroadcaster.h"
#include "AnalogReader.h"
#include "HCSR04Reader.h"

class SensorValueEvent
{
public:
    String niceName;
    int rawValue;
    float normValue;
    SensorValueEvent(String niceName, int rawValue, float normValue) : niceName(niceName), rawValue(rawValue), normValue(normValue) {}
};

class SensorManager : public Manager,
                        public EventBroadcaster<SensorValueEvent>
{
public:
    SensorManager();
    /*
    using EventBroadcaster<BatteryEvent>::addListener;
    using EventBroadcaster<BatteryEvent>::sendEvent;
    using EventBroadcaster<AnalogEvent>::addListener;
    using EventBroadcaster<AnalogEvent>::sendEvent;
    using EventBroadcaster<SensorValueEvent>::addListener;
    using EventBroadcaster<SensorValueEvent>::sendEvent;
*/
    void initManager();
    void update();
    
  bool handleCommand(OSCMessage &command) override;

    void registerAnalogReader(String niceName, byte pin);
    void registerHCSR04Reader(String niceName, byte triggerPin, byte echoPin, boolean active);

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