#include "MotorShield2Manager.h"

#ifdef HAS_MOTORWING
MotorShield2Manager::MotorShield2Manager() : Manager("motorwing")
{
    serialDebug = MOTORS_DEBUG;
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

    for (auto &pair : steppers)
    {
        float pos = pair.second->currentPosition();
        pair.second->update();
        long newPos = pair.second->currentPosition();
        float speed = pair.second->currentSpeed();
        float maxSpeed = pair.second->maxSpeed();

        if (millis() - lastEventTime > 5)
        {
            if (newPos != lastEventPos)
            {
                sendEvent(StepperEvent(StepperEvent::Type::MOVED, pair.first, newPos, speed, maxSpeed));
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

    dcMotors.insert({port, new DCMotor(AFMS.getMotor(port), (int)port, 50)});
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

    if (MOTORS_DEBUG) compDebug("set DC#" + String(port) + " max speed " + String(value));
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

void MotorShield2Manager::registerStepper(byte index, byte pin1, byte pin2, byte pin3, byte pin4)
{
    if (!checkInit())
        return;

    // TODO replace with registerPins
    std::set<int> pins{pin1, pin2, pin3, pin4};

    for (int pin : pins)
    {
        if (Component::forbiddenPins.find(pin) != Component::forbiddenPins.end())
        {
            compError("cannot register stepper: " + String(pin) + " is already used !");
            return;
        }
        Component::forbiddenPins.insert(pin);
    }

    // FIXME id not safe
    registerStepper(index, pin1 | pin2 | pin3 | pin4, new AccelStepper(AccelStepper::FULL4WIRE, pin1, pin2, pin3, pin4));
}

void MotorShield2Manager::registerShieldv2Stepper(byte index, int steps, StepperPort port)
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
        registerStepper(index, 1, new AccelStepper(std::bind(&MotorShield2Manager::forward1, this), std::bind(&MotorShield2Manager::backward1, this)));

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
        registerStepper(index, 2, new AccelStepper(std::bind(&MotorShield2Manager::forward2, this), std::bind(&MotorShield2Manager::backward2, this)));

        break;
    }
}

void MotorShield2Manager::registerStepper(byte index, byte id, AccelStepper *stepper)
{
    if (steppers.count(index) > 0)
    {
        compError("cannot register stepper#" + String(index));
        return;
    }
    steppers.insert({index, new StepperMotor(id, stepper)});
    steppers[index]->initComponent(serialDebug);
    compDebug("registered " + steppers[index]->name + " on #" + String(index));
}

void MotorShield2Manager::stepperGoTo(byte index, long value)
{
    if (!checkInit())
        return;

    if (steppers.count(index) == 0)
    {
        compError("no stepper on " + String(index));
        return;
    }

    if (MOTORS_DEBUG) compDebug("stepper " + String(index) + " go to " + String(value));
    steppers[index]->goTo(value);
}

void MotorShield2Manager::stepperMove(byte index, long value)
{
    if (!checkInit())
        return;

    if (steppers.count(index) == 0)
    {
        compError("no stepper on " + String(index));
        return;
    }

    // REFACTOR make StepperMotor methods
    if (MOTORS_DEBUG) compDebug("stepper " + String(index) + " move relative " + String(value));
    steppers[index]->moveTo(value);
}

void MotorShield2Manager::stepperReset(byte index)
{
    if (!checkInit())
        return;

    if (steppers.count(index) == 0)
    {
        compError("no stepper on " + String(index));
        return;
    }

    if (MOTORS_DEBUG) compDebug("stepper " + String(index) + " reset ");
    // REFACTOR make StepperMotor methods
    steppers[index]->reset();
}

void MotorShield2Manager::stepperSetSpeed(byte index, float value)
{
    if (!checkInit())
        return;

    if (steppers.count(index) == 0)
    {
        compError("no stepper on " + String(index));
        return;
    }

    if (MOTORS_DEBUG) compDebug("stepper " + String(index) + " set speed " + String(value));
    steppers[index]->setSpeed(value);
}

void MotorShield2Manager::stepperSetSpeedRel(byte index, float value)
{
    if (!checkInit())
        return;

    if (steppers.count(index) == 0)
    {
        compError("no stepper on " + String(index));
        return;
    }

    if (MOTORS_DEBUG) compDebug("stepper " + String(index) + " set speed rel " + String(value));
    steppers[index]->setSpeedRel(value);
}

void MotorShield2Manager::stepperSetAccel(byte index, float value)
{
    if (!checkInit())
        return;

    if (steppers.count(index) == 0)
    {
        compError("no stepper on " + String(index));
        return;
    }

    if (MOTORS_DEBUG) compDebug("stepper " + String(index) + " set accel " + String(value));
    steppers[index]->setAcceleration(value);
}

void MotorShield2Manager::stepperSetMaxSpeed(byte index, float value)
{
    if (!checkInit())
        return;

    if (steppers.count(index) == 0)
    {
        compError("no stepper on " + String(index));
        return;
    }

    if (MOTORS_DEBUG) compDebug("stepper " + String(index) + " set max speed " + String(value));
    steppers[index]->setMaxSpeed(value);
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
            dcMaxSpeed((DCPort)index, (int)value);
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
    return false;
}
#endif