#include "ServoManager.h"

ServoManager::ServoManager() : Manager("servo")
{
  serialDebug = SERVO_DEBUG;
}

void ServoManager::initManager()
{
  Manager::initManager();
}

void ServoManager::update()
{
  if (!checkInit())
    return;
  // nothing
}

void ServoManager::registerServo(int index, int pin, int min, int max, int start)
{
  if (!checkInit())
    return;

  std::set<int> servoPins = {2, 4, 12, 13, 14, 15, 16, 17, 21, 22, 23, 25, 32};

  if (SERVO_ALLOW_UNSAFE_PINS)
    if (servoPins.find(pin) == servoPins.end())
    {
      compError("pin " + String(pin) + " is not valid to attach Servo !");
      return;
    }

  // TODO replace with registerPin
  if (Component::forbiddenPins.find(pin) != Component::forbiddenPins.end())
  {
    compError("cannot register prop: " + String(pin) + " is already used !");
    return;
  }
  Component::forbiddenPins.insert(pin);

  servos.insert({index, new ServoMotor(pin, min, max, start)});
  servos[index]->initComponent(serialDebug);

  compDebug("register prop: " + String(servos[index]->name));
}

void ServoManager::setServoAbs(int index, int value)
{
  if (servos.count(index) == 0)
  {
    compError("incorrect index: " + String(index));
    return;
  }
  compDebug("set servo#" + String(index) + " abs: " + String(value));
  servos[index]->setAbs(value);
}

void ServoManager::setServoRel(int index, float value)
{
  if (servos.count(index) == 0)
  {
    compError("incorrect index: " + String(index));
    return;
  }
  compDebug("set servo#" + String(index) + " rel: " + String(value));
  servos[index]->setRel(value);
}

void ServoManager::setServoMin(int index, int value)
{
  if (servos.count(index) == 0)
  {
    compError("incorrect index: " + String(index));
    return;
  }
  compDebug("set servo#" + String(index) + " min: " + String(value));
  servos[index]->setMin(value);
}

void ServoManager::setServoMax(int index, int value)
{
  if (servos.count(index) == 0)
  {
    compError("incorrect index: " + String(index));
    return;
  }
  compDebug("set servo#" + String(index) + " max: " + String(value));
  servos[index]->setMax(value);
}

void ServoManager::setServoMin(int index, float value)
{
  if (servos.count(index) == 0)
  {
    compError("incorrect index: " + String(index));
    return;
  }
  compDebug("set servo#" + String(index) + " min: " + String(value));
  servos[index]->setMin(value);
}

void ServoManager::setServoMax(int index, float value)
{
  if (servos.count(index) == 0)
  {
    compError("incorrect index: " + String(index));
    return;
  }
  compDebug("set servo#" + String(index) + " max: " + String(value));
  servos[index]->setMax(value);
}

void ServoManager::setServoInverse(int index, bool value)
{
  if (servos.count(index) == 0)
  {
    compError("incorrect index: " + String(index));
    return;
  }
  compDebug("set servo#" + String(index) + " inverse: " + String(value));
  servos[index]->setInverse(value);
}

bool ServoManager::handleCommand(OSCMessage &command)
{
  if (!checkInit())
    return false;

  char buf[32];
  command.getAddress(buf);
  String address = String(buf);
  compLog("handle command: " + address);

  if (address.equals("/servo"))
  {
    if (checkCommandArguments(command, "ii", false))
    {
      int index = command.getInt(0);
      int value = command.getInt(1);
      setServoAbs(index, value);
      return true;
    }
    else if (checkCommandArguments(command, "if", true))
    {
      int index = command.getInt(0);
      float value = command.getFloat(1);
      setServoRel(index, value);
      return true;
    }
  }
  if (address.equals("/servo/min"))
  {
    if (checkCommandArguments(command, "ii", false))
    {
      int index = command.getInt(0);
      int value = command.getInt(1);
      setServoMin(index, value);
      return true;
    }
    else if (checkCommandArguments(command, "if", true))
    {
      int index = command.getInt(0);
      float value = command.getFloat(1);
      setServoMin(index, value);
      return true;
    }
  }
  if (address.equals("/servo/max"))
  {
    if (checkCommandArguments(command, "ii", false))
    {
      int index = command.getInt(0);
      int value = command.getInt(1);
      setServoMin(index, value);
      return true;
    }
    else if (checkCommandArguments(command, "if", true))
    {
      int index = command.getInt(0);
      float value = command.getFloat(1);
      setServoMax(index, value);
      return true;
    }
  }
  if (address.equals("/servo/inverse"))
  {
    if (checkCommandArguments(command, "ii", false))
    {
      int index = command.getInt(0);
      bool value = command.getInt(1)>0;
      setServoInverse(index, value);
      return true;
    }
    else if (checkCommandArguments(command, "ib", true))
    {
      int index = command.getInt(0);
      bool value = command.getBoolean(1);
      setServoInverse(index, value);
      return true;
    }
  }
  return false;
}