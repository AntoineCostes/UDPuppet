#include "StepperMotor.h"

#ifdef HAS_MOTORWING
StepperMotor::StepperMotor(byte id, AccelStepper *stepper) : Component("stepper_" + String(id)),
                                                             stepper(stepper),
                                                             mode(SPEED)
{
    // TODO values in constructor ?
    // FIXME maximum values for motorwing ?
    floatParameters["maxSpeed"] = 100; 
    floatParameters["acceleration"] = 1000;
    mode = SPEED;
}

void StepperMotor::initComponent(bool serialDebug)
{
    Component::initComponent(serialDebug);
    // setSpeed(5000.0f);
    //moveTo(500.0f);
    stepper->setMaxSpeed(floatParameters["maxSpeed"]);
    stepper->setAcceleration(floatParameters["acceleration"]);
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

    // ensure the stepper moves
    if (stepper->targetPosition() == value)
        stepper->moveTo(value+100);


    // make sure previous speed does not interfere
    if (mode != POSITION)
    {
        stepper->setSpeed(0);
        mode = POSITION;
    }
    stepper->moveTo(value);
    compDebug("go to " + String(value));
}

void StepperMotor::moveTo(long value)
{
    if (!checkInit())
        return;

    
    // make sure previous speed does not interfere
    if (mode != POSITION)
    {
        stepper->setSpeed(0);
        mode = POSITION;
    }
    stepper->move(value);
    compDebug("move to " + String(value));
}

void StepperMotor::reset()
{
    if (!checkInit())
        return;

    // TODO restore previous state ? 
    //float speed = stepper->speed();
    stepper->setCurrentPosition(0);
    //stepper->setSpeed(speed); // does not work
    compDebug("reset");
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

    // setAcceleration triggers run to position
    if (stepper->speed() == 0)
        stepper->move(0); // cancel target position

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
    
    // setMaxSpeed triggers run to position
    if (stepper->speed() == 0)
        stepper->move(0); // cancel target position

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

float StepperMotor::maxSpeed()
{
   return stepper->maxSpeed();
}

// float StepperMotor::acceleration()
// {
//    return stepper->acceleration();
// }
#endif