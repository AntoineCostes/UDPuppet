#include "../common/Manager.h"
#include "../utils/EventBroadcaster.h"
#include "AnalogReader.h"

class AnalogEvent
{
public:
    String niceName;
    int rawValue;
    float normValue;
    AnalogEvent(String niceName, int rawValue, float normValue) : niceName(niceName), rawValue(rawValue), normValue(normValue) {}
};

class AnalogManager : public Manager,
                        public EventBroadcaster<AnalogEvent>
{
public:
    AnalogManager();

    void update();

    void registerAnalogReader(String niceName, int pin);

protected:
    // props
};