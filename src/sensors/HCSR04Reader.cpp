#include "HCSR04Reader.h"

HCSR04Reader::HCSR04Reader(byte triggerPin, byte echoPin, String niceName) : Component("hcsr04[" + String(triggerPin)+"-"+String(echoPin)),
                                                                            triggerPin(triggerPin),
                                                                            echoPin(echoPin),
                                                                            distanceValueMm(-1),
                                                                            niceName(niceName),
                                                                            active(true)
{

}

void HCSR04Reader::initComponent(bool serialDebug)
{
    pinMode(triggerPin, OUTPUT); 
    pinMode(echoPin, INPUT); 
    Component::initComponent(serialDebug);
}

// TODO parallel thread
void HCSR04Reader::update()
{
    compDebug("HCSR04");
    if (active)
    {
        // Clears the trigPin
        digitalWrite(triggerPin, LOW);
        delayMicroseconds(2);
        // Sets the trigPin on HIGH state for 10 micro seconds
        digitalWrite(triggerPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(triggerPin, LOW);
        // Reads the echoPin, returns the sound wave travel time in microseconds
        long value = pulseIn(echoPin, HIGH)*0.034/2;

        if (value != distanceValueMm)
        {
            // send event (parallel thread)
            distanceValueMm = value;
        }
    }
    else
        distanceValueMm = -1;
}