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

    for (auto const &prop : props)
    {   
        AnalogReader* reader = static_cast<AnalogReader*>(prop.get());

        uint16_t raw = reader->read();
        float norm = float(raw) / 1024.0f;

        sendEvent(AnalogEvent(reader->niceName, raw, norm));
    }
}