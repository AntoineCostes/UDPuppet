#include "AnalogReader.h"


AnalogReader::AnalogReader(byte pin, String niceName) : Component("reader" + String(pin)), 
                                                            pin(pin),
                                                            niceName(niceName),
                                                            lastValue(0)
{
}
        
void AnalogReader::initComponent(bool serialDebug)
{
    Component::initComponent(serialDebug);
}

void AnalogReader::update()
{

}

int AnalogReader::read()
{
    uint16_t currentValue = analogRead(pin);

    if (currentValue == lastValue)
    {
        return -1;
    }
    lastValue = currentValue;
    return currentValue;
}