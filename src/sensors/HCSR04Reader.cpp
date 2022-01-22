#include "HCSR04Reader.h"

HCSR04Reader::HCSR04Reader(int triggerPin, int echoPin, String niceName, boolean active) : Component("hcsr04[" + String(triggerPin)+"-"+String(echoPin)),
                                                                            triggerPin(triggerPin),
                                                                            echoPin(echoPin),
                                                                            distanceValueMm(-1),
                                                                            maxValue(0),
                                                                            normValue(0),
                                                                            niceName(niceName),
                                                                            active(active)
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
        long value = int(pulseIn(echoPin, HIGH)*0.034/2);

        if (value != distanceValueMm)
            gotNewValue = true;
        
        distanceValueMm = value;
        maxValue = max(maxValue, distanceValueMm);
        normValue = float(distanceValueMm)/float(maxValue);
    }
    else
    {
        gotNewValue = false;
        distanceValueMm = -1;
        maxValue = 0;
        normValue = 0.0f;
    }
}