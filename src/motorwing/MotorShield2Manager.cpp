#include "MotorShield2Manager.h"

#ifdef HAS_MOTORWING
MotorShield2Manager::MotorShield2Manager() : Manager("motorwing")
{
    serialDebug = MOTORWING_DEBUG;
}

void MotorShield2Manager::initManager()
{
    Manager::initManager();
    AFMS.begin();
    Wire.setClock(400000L);
    lastEventPos = 0;
    lastEventTime = 0;
}

void MotorShield2Manager::update()
{
    if (!checkInit())
        return;

    
    for(std::size_t i = 0; i < steppers.size(); ++i)
    //for (auto &stepper : steppers) // TODO include index in name
    {
        steppers[i]->update();

        if (millis() - lastEventTime > 50)
        {
            long newPos = steppers[i]->currentPosition();

            if (newPos != lastEventPos)
            {
                float speed = steppers[i]->currentSpeed();
                float maxSpeed = steppers[i]->maxSpeed();
                sendEvent(StepperEvent(StepperEvent::Type::MOVED, i, newPos, speed, maxSpeed));
                lastEventPos = newPos;
                lastEventTime = millis();
            }
        }

    
        // TODO : other types of event ? changed parameter ?
    }
}   

void MotorShield2Manager::addDCMotor(DCPort port)
{
    if (!checkInit())
        return;

    // TODO replace with dcMotors.count && check steppers ports
    if (usedPorts.count(port) > 0)
    {
        compError("cannot add DC motor : " + String(port) + " is already used !");
        return;
    }
    usedPorts.insert(port);

    dcMotors.insert({port, new DCMotor(AFMS.getMotor(port), port, 50)});
    dcMotors[port]->initComponent(serialDebug);
    //dc.insert({index, AFMS.getMotor(port)});
    // compDebug("added DC#" + String(index) + " on port " + String(port));
    compDebug("added DC#" + String(port));
}

void MotorShield2Manager::dcRun(DCPort port, float value)
{
    if (!checkInit())
        return;

    // if (dc.count(index) == 0)
    // {
    //     compError("no DC on " + String(index));
    //     return;
    // }

    if (dcMotors.count(port) == 0)
    {
        compError("no DC on " + String(port));
        return;
    }
    dcMotors[port]->run(value);

    // if (value >= 0)
    // {
    // dc[index]->setSpeed(value);
    // dc[index]->run(FORWARD);
    // }
    // else if (value < 0)
    // {
    // dc[index]->setSpeed(abs(value));
    // dc[index]->run(BACKWARD);
    // }
    //else dc[index]->run(RELEASE);

    // compDebug("set DC#" + String(port) + " speed " + String(value));
}

void MotorShield2Manager::dcMaxSpeed(DCPort port, int value)
{
    if (!checkInit())
        return;

    if (dcMotors.count(port) == 0)
    {
        compError("no DC on " + String(port));
        return;
    }
    dcMotors[port]->setMaxSpeed(value);

    if (MOTORWING_DEBUG) compDebug("set DC#" + String(port) + " max speed " + String(value));
}

void MotorShield2Manager::dcStopAll()
{
    // for (int i = 0; i < dc.size(); i++)
    // {
    //     dcStop(i);
    // }
    for (int i = 0; i < dcMotors.size(); i++)
    {
        dcStop((DCPort)i);
    }
}

void MotorShield2Manager::dcStop(DCPort port)
{
    if (!checkInit())
        return;

    // if (dc.count(index) == 0)
    // {
    //     compError("no DC on " + String(index));
    //     return;
    // }
    // dc[index]->run(RELEASE);
    if (dcMotors.count(port) == 0)
    {
        compError("no DC on " + String(port));
        return;
    }
    dcMotors[port]->stop();
    compDebug("stop DC#" + String(port));
}

void MotorShield2Manager::registerStepper(int pin1, int pin2, int pin3, int pin4)
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

void MotorShield2Manager::registerStepper(int step, int dir)
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

void MotorShield2Manager::registerShieldv2Stepper(int steps, StepperPort port)
{
    if (!checkInit())
        return;

    compDebug("register Stepper on port " + String(port));
    switch (port)
    {
    case M1_M2:
        if (usedPorts.find(DCPort::M1) != usedPorts.end() || usedPorts.find(DCPort::M2) != usedPorts.end())
        {
            compError("cannot register Stepper motor : " + String(port) + " is already used !");
            return;
        }
        usedPorts.insert(DCPort::M1);
        usedPorts.insert(DCPort::M2);

        shieldStepper1 = AFMS.getStepper(steps, 1);
        registerStepper(1, new AccelStepper(std::bind(&MotorShield2Manager::forward1, this), std::bind(&MotorShield2Manager::backward1, this)));

        break;

    case M3_M4:
        if (usedPorts.find(DCPort::M3) != usedPorts.end() || usedPorts.find(DCPort::M4) != usedPorts.end())
        {
            compError("cannot register Stepper motor : " + String(port) + " is already used !");
            return;
        }
        usedPorts.insert(DCPort::M3);
        usedPorts.insert(DCPort::M4);

        shieldStepper2 = AFMS.getStepper(steps, 2);
        registerStepper(2, new AccelStepper(std::bind(&MotorShield2Manager::forward2, this), std::bind(&MotorShield2Manager::backward2, this)));

        break;
    }
}

void MotorShield2Manager::registerStepper(int id, AccelStepper *stepper)
{
    steppers.emplace_back(new StepperMotor(id, stepper));
    steppers.back()->initComponent(serialDebug);
    compDebug("registered " + steppers.back()->name);
}

void MotorShield2Manager::stepperGoTo(int index, long value)
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

void MotorShield2Manager::stepperMove(int index, long value)
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

void MotorShield2Manager::stepperReset(int index)
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

void MotorShield2Manager::stepperSetSpeed(int index, float value)
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

void MotorShield2Manager::stepperSetSpeedRel(int index, float value)
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

void MotorShield2Manager::stepperSetAccel(int index, float value)
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

void MotorShield2Manager::stepperSetMaxSpeed(int index, float value)
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

void MotorShield2Manager::stepperRelease(int index)
{
    if (!checkInit())
        return;

    if (index < 0 || index >= steppers.size())
    {
        compError("incorrect index " + String(index));
        return;
    }
    if (MOTORWING_DEBUG) compDebug("release stepper " + String(index));
    if (steppers[index]->name.equals("stepper_1"))
    {   
        compDebug("release 1");
        shieldStepper1->release();
    }
    else if (steppers[index]->name.equals("stepper_2"))
    {   
        compDebug("release 2");
        shieldStepper2->release();
    }
}


void MotorShield2Manager::forward1()
{
    shieldStepper1->onestep(FORWARD, DOUBLE);
}

void MotorShield2Manager::backward1()
{
    shieldStepper1->onestep(BACKWARD, DOUBLE);
}

void MotorShield2Manager::forward2()
{
    shieldStepper2->onestep(FORWARD, DOUBLE);
}

void MotorShield2Manager::backward2()
{
    shieldStepper2->onestep(BACKWARD, DOUBLE);
}

bool MotorShield2Manager::handleCommand(OSCMessage &command)
{
    if (!checkInit())
        return false;

    char buf[32];
    command.getAddress(buf);
    String address = String(buf);
    compLog("handle command : " + address);

    if (address.equals("/dc/run")) // FIXME name + "/run"
    {
        if (checkCommandArguments(command, "if", true))
        {
            int index = command.getInt(0);
            float value = command.getFloat(1);
            dcRun((DCPort)index, value);
            return true;
        }
    }
    else if (address.equals("/dc/stop"))
    {
        if (command.size() == 0)
        {
            dcStopAll();
            return true;
        }
        else if (checkCommandArguments(command, "i", true))
        {
            int index = command.getInt(0);
            dcStop((DCPort)index);
            return true;
        }
    }
    else if (address.equals("/dc/maxspeed")) // FIXME name + "/run"
    {
        if (checkCommandArguments(command, "ii", false))
        {
            int index = command.getInt(0);
            int value = command.getInt(1);
            dcMaxSpeed((DCPort)index, value);
            return true;
        }
        else if (checkCommandArguments(command, "if", true))
        {
            int index = command.getInt(0);
            float value = command.getFloat(1);
            dcMaxSpeed((DCPort)index, value);
            return true;
        }
    }
    else if (address.equals("/stepper/reset"))
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