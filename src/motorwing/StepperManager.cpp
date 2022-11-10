#include "StepperManager.h"

#ifdef HAS_STEPPER_DRIVER
StepperManager::StepperManager() : Manager("stepper")
{
    serialDebug = STEPPER_DEBUG;
}

void StepperManager::initManager()
{
    Manager::initManager();
    lastEventPos = 0;
    lastEventTime = 0;
}

void StepperManager::update()
{
    if (!checkInit())
        return;

    
    for(std::size_t i = 0; i < steppers.size(); ++i)
    //for (auto &stepper : steppers) // TODO include index in name
    {
        steppers[i]->update();

        if (millis() - lastEventTime > 500)
        {
            long newPos = steppers[i]->currentPosition();

            if (newPos != lastEventPos)
            {
                float speed = steppers[i]->currentSpeed();
                float maxSpeed = steppers[i]->maxSpeed();
                sendEvent(StepperEvent2(StepperEvent2::Type::MOVED, i, newPos, speed, maxSpeed));
                lastEventPos = newPos;
                lastEventTime = millis();
            }
        }
        // TODO : other types of event ? changed parameter ?
    }
}   

void StepperManager::registerStepper(int pin1, int pin2, int pin3, int pin4)
{
    if (!checkInit())
        return;

    std::set<int> pins{pin1, pin2, pin3, pin4};

    if (!Component::registerPins(pins))
    {
        compError("cannot register stepper!");
        return;
    }

    // FIXME id not safe
    registerStepper(pin1 | pin2 | pin3 | pin4, new AccelStepper(AccelStepper::FULL4WIRE, pin1, pin2, pin3, pin4));
}

void StepperManager::registerStepper(int step, int dir)
{
    if (!checkInit())
        return;

    std::set<int> pins{step, dir};

    if (!Component::registerPins(pins))
    {
        compError("cannot register stepper!");
        return;
    }

    // FIXME id not safe
    registerStepper(step | dir, new AccelStepper(AccelStepper::DRIVER, step ,dir));
}

void StepperManager::registerStepper(int step, int dir, int enable)
{
    if (!checkInit())
        return;

    std::set<int> pins{step, dir, enable};

    if (!Component::registerPins(pins))
    {
        compError("cannot register stepper!");
        return;
    }

    // FIXME id not safe
    registerStepper(step | dir, new AccelStepper(AccelStepper::DRIVER, step ,dir), enable);
}

void StepperManager::registerStepper(int id, AccelStepper *stepper)
{
    steppers.emplace_back(new StepperMotor(id, stepper));
    steppers.back()->initComponent(serialDebug);
    compDebug("registered " + steppers.back()->name);
}

void StepperManager::registerStepper(int id, AccelStepper *stepper, int enablePin)
{
    steppers.emplace_back(new StepperMotor(id, stepper, enablePin));
    steppers.back()->initComponent(serialDebug);
    compDebug("registered " + steppers.back()->name + " with enable pin");
}

void StepperManager::stepperGoTo(int index, long value)
{
    if (!checkInit())
        return;

    if (index < 0 || index >= steppers.size())
    {
        compError("incorrect index " + String(index));
        return;
    }

    if (MOTORWING_DEBUG) compDebug("stepper " + String(index) + " go to " + String(value));
    steppers[index]->goTo(value);
}

void StepperManager::stepperMove(int index, long value)
{
    if (!checkInit())
        return;

    if (index < 0 || index >= steppers.size())
    {
        compError("incorrect index " + String(index));
        return;
    }

    // REFACTOR make StepperMotor methods
    if (MOTORWING_DEBUG) compDebug("stepper " + String(index) + " move relative " + String(value));
    steppers[index]->moveTo(value);
}

void StepperManager::stepperReset(int index)
{
    if (!checkInit())
        return;

    if (index < 0 || index >= steppers.size())
    {
        compError("incorrect index " + String(index));
        return;
    }

    if (MOTORWING_DEBUG) compDebug("stepper " + String(index) + " reset ");
    // REFACTOR make StepperMotor methods
    steppers[index]->reset();
}

void StepperManager::stepperSetSpeed(int index, float value)
{
    if (!checkInit())
        return;

    if (index < 0 || index >= steppers.size())
    {
        compError("incorrect index " + String(index));
        return;
    }

    if (MOTORWING_DEBUG) compDebug("stepper " + String(index) + " set speed " + String(value));
    steppers[index]->setSpeed(value);
}

void StepperManager::stepperSetSpeedRel(int index, float value)
{
    if (!checkInit())
        return;

    if (index < 0 || index >= steppers.size())
    {
        compError("incorrect index " + String(index));
        return;
    }

    if (MOTORWING_DEBUG) compDebug("stepper " + String(index) + " set speed rel " + String(value));
    steppers[index]->setSpeedRel(value);
}

void StepperManager::stepperSetAccel(int index, float value)
{
    if (!checkInit())
        return;

    if (index < 0 || index >= steppers.size())
    {
        compError("incorrect index " + String(index));
        return;
    }

    if (MOTORWING_DEBUG) compDebug("stepper " + String(index) + " set accel " + String(value));
    steppers[index]->setAcceleration(value);
}

void StepperManager::stepperSetMaxSpeed(int index, float value)
{
    if (!checkInit())
        return;

    if (index < 0 || index >= steppers.size())
    {
        compError("incorrect index " + String(index));
        return;
    }

    if (MOTORWING_DEBUG) compDebug("stepper " + String(index) + " set max speed " + String(value));
    steppers[index]->setMaxSpeed(value);
}


void StepperManager::stepperRelease(int index)
{
    if (!checkInit())
        return;

    steppers[index]->enableOutputs();
}

bool StepperManager::handleCommand(OSCMessage &command)
{
    if (!checkInit())
        return false;

    char buf[32];
    command.getAddress(buf);
    String address = String(buf);
    compDebug("handle command : " + address);

    if (address.equals("/stepper/reset"))
    {
        if (checkCommandArguments(command, "i", true))
        {
            int index = command.getInt(0);
            stepperReset(index);
            return true;
        }
    }
    else if (address.equals("/stepper/move"))
    {
        if (checkCommandArguments(command, "ii", true))
        {
            int index = command.getInt(0);
            int value = command.getInt(1);
            stepperMove(index, value);
            return true;
        }
    }
    else if (address.equals("/stepper/go"))
    {
        if (checkCommandArguments(command, "ii", true))
        {
            int index = command.getInt(0);
            int value = command.getInt(1);
            stepperGoTo(index, value);
            return true;
        }
    }
    else if (address.equals("/stepper/speed"))
    {
        if (checkCommandArguments(command, "if", true))
        {
            int index = command.getInt(0);
            float value = command.getFloat(1);
            stepperSetSpeed(index, value);
            return true;
        }
    }
    else if (address.equals("/stepper/speedrel"))
    {
        if (checkCommandArguments(command, "if", true))
        {
            int index = command.getInt(0);
            float value = command.getFloat(1);
            stepperSetSpeedRel(index, value);
            return true;
        }
    }
    else if (address.equals("/stepper/maxspeed"))
    {
        if (checkCommandArguments(command, "if", true))
        {
            int index = command.getInt(0);
            float value = command.getFloat(1);
            stepperSetMaxSpeed(index, (float)value);
            return true;
        }
    }
    else if (address.equals("/stepper/accel"))
    {
        if (checkCommandArguments(command, "if", true))
        {
            int index = command.getInt(0);
            float value = command.getFloat(1);
            stepperSetAccel(index, value);
            return true;
        }
    }
    else if (address.equals("/stepper/release"))
    {
        if (checkCommandArguments(command, "i", true))
        {
            int index = command.getInt(0);
            stepperRelease(index);
            return true;
        }
    }
    return false;
}
#endif