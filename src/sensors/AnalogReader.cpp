#include "AnalogReader.h"


AnalogReader::AnalogReader(int pin, String niceName) : Component("reader" + String(pin)), 
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
    uint16_t current = analogRead(pin);
    //compDebug(String(current));

    if (current == lastValue)
    {
        return -1;
    }
    lastValue = current;
    return current;
}