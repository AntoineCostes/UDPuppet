#include "DCMotor.h"

#ifdef HAS_MOTORWING
DCMotor::DCMotor(Adafruit_DCMotor* motor, int port, int maxSpeed):Component("dc_M" + String(port)), motor(motor), maxSpeed(maxSpeed)
{
    intParameters["maxSpeed"] = maxSpeed;
}

void DCMotor::initComponent(bool serialDebug)
{
    Component::initComponent(serialDebug);
}

void DCMotor::update()
{

}

void DCMotor::run(float value)
{
    if (abs(value) > 1.0f)
    {
        compError("incorrect value :" + String(value));
        return;
    }
    compDebug("set speed " + String(value*maxSpeed));
    if (value >= 0)
    {
        motor->setSpeed(value*maxSpeed);
        motor->run(FORWARD);
    }
    else if (value < 0)
    {
        motor->setSpeed(abs(value*maxSpeed));
        motor->run(BACKWARD);
    }
}

void DCMotor::stop()
{
    compDebug("stop");
    motor->run(RELEASE);
}

void DCMotor::setMaxSpeed(int value)
{
    compDebug("set max speed: " + String(value));
    maxSpeed = value;  
    intParameters["maxSpeed"] = value;
    overrideFlashParameters();
}
#endif