#include "AnalogReader.h"


AnalogReader::AnalogReader(int pin, String niceName) : Component("reader" + String(pin)), 
                                                            pin(pin),
                                                            niceName(niceName)
{
}
        
void AnalogReader::initComponent(bool serialDebug)
{
    Component::initComponent(serialDebug);
}

void AnalogReader::update()
{

}

uint16_t AnalogReader::read()
{
    return analogRead(pin);
}