#include "StepperMotor.h"

StepperMotor::StepperMotor(byte id, AccelStepper *stepper) : Component("stepper_" + String(id)),
                                                             stepper(stepper),
                                                             maxSpeed(1000),
                                                             acceleration(300),
                                                             mode(SPEED)
{
    floatParameters["maxSpeed"] = maxSpeed;
    floatParameters["acceleration"] = acceleration;
    mode = SPEED;
}

void StepperMotor::initComponent(bool serialDebug)
{
    Component::initComponent(serialDebug);
    // setSpeed(5000.0f);
    //moveTo(500.0f);
    startValue = stepper->currentPosition();
    stepper->setMaxSpeed(maxSpeed);
    stepper->setAcceleration(acceleration);
}

void StepperMotor::update()
{
    if (!checkInit())
        return;

    switch (mode)
    {
    case POSITION:
        stepper->run();
        break;

    case SPEED:
        stepper->runSpeed();
        break;
    }
}

void StepperMotor::goTo(long value)
{
    if (!checkInit())
        return;

    mode = POSITION;
    stepper->moveTo(value);
}

void StepperMotor::goToFromStart(long value)
{
    if (!checkInit())
        return;

    mode = POSITION;
    stepper->moveTo(value +  startValue);
    compDebug("go to " + String(value));
}

void StepperMotor::moveTo(long value)
{
    if (!checkInit())
        return;

    mode = POSITION;
    stepper->moveTo(stepper->currentPosition() + value);
    compDebug("move to " + String(value));
}

void StepperMotor::reset()
{
    if (!checkInit())
        return;

    startValue = stepper->currentPosition();
    compDebug("reset ");
}

void StepperMotor::setSpeed(float value)
{
    if (!checkInit())
        return;

    mode = SPEED;
    stepper->setSpeed(value);
    compDebug("set speed " + String(value));
}

void StepperMotor::setSpeedRel(float value)
{
    if (!checkInit())
        return;

  if (value < -1 || value > 1)
  {
    compError("can't set rel speed, incorrect value: " + String(value));
    return;
  }

    mode = SPEED;
    stepper->setSpeed(value * stepper->maxSpeed());
    compDebug("set rel speed " + String(value * stepper->maxSpeed()));
}

void StepperMotor::setAcceleration(float value)
{
    if (!checkInit())
        return;

    if (value < 0 )
    {
        compError("can't set acceleration, incorrect value: " + String(value));
        return;
    }
    compDebug("set accel " + String(value));
    stepper->setAcceleration(value);
    floatParameters["acceleration"] = value;
    overrideFlashParameters();
}

void StepperMotor::setMaxSpeed(float value)
{
    if (!checkInit())
        return;

    if (value < 0 )
    {
        compError("can't set max speed, incorrect value: " + String(value));
        return;
    }

    compDebug("set max speed " + String(value));
    stepper->setMaxSpeed(value);
    floatParameters["maxSpeed"] = value;
    overrideFlashParameters();
}

long StepperMotor::currentPosition()
{
   return stepper->currentPosition();
}

float StepperMotor::currentSpeed()
{
   return stepper->speed();
}
