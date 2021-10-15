#include "AnalogManager.h"

AnalogManager::AnalogManager() : Manager("analog")
{
    serialDebug = MASTER_DEBUG;
}

void AnalogManager::registerAnalogReader(String niceName, int pin)
{    
    std::set<int> pins { pin };
    if (Manager::registerProp(new AnalogReader(pin, niceName), pins))
    {
        compDebug("reading " + niceName + " on " + String(pin));
    }
}

void AnalogManager::update()
{
    if (!checkInit())
        return;

    //uint16_t val = analogRead(A0);

    //compDebug(String(val));
    for (auto const &prop : props)
    {   
        AnalogReader* reader = static_cast<AnalogReader*>(prop.get());

        int raw = reader->read();   

        if (raw >= 0) // if new value
        {   
            float norm = float(raw) / 1024.0f;
            sendEvent(AnalogEvent(reader->niceName, raw, norm));
        }
    }
}