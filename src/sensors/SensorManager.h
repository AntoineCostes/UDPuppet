#pragma once
#include "../common/Manager.h"
#include "../utils/EventBroadcaster.h"
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
    void initManager();
    void update();
    
    bool handleCommand(OSCMessage &command) override;

    void registerHCSR04(String niceName, byte triggerPin, byte echoPin, boolean active);

protected:
    std::vector<HCSR04Reader*> ultrasonics; 
};