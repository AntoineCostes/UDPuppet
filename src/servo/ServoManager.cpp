#include "ServoManager.h"

#ifdef NUM_SERVOS
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

bool ServoManager::checkServoPin(byte pin)
{
  std::set<int> servoPins = {2, 4, 12, 13, 14, 15, 16, 17, 21, 22, 23, 25, 32};
  #ifdef ESP32
  if (!SERVO_ALLOW_UNSAFE_PINS)
    if (servoPins.find(pin) == servoPins.end())
    {
      compError("pin " + String(pin) + " is not valid to attach Servo !");
      return false;
    }
  #endif

  if (!Component::registerPin((int)pin))
  {
      compError("cannot register servo !");
      return false;
  } 
  return true;
}

void ServoManager::registerServo(byte index, byte pin, float startPosition)
{
  if (!checkInit())
    return;
  
  if (!checkServoPin((int)pin))
    return;

  servos.insert({(int)index, new ServoMotor(pin, startPosition)});
  servos[index]->initComponent(serialDebug);

  compDebug("register prop: " + String(servos[index]->name));
}

void ServoManager::registerServo(byte index, byte pin, float min, float max, float startPosition)
{
  if (!checkInit())
    return;
  
  if (!checkServoPin((int)pin))
    return;

  servos.insert({(int)index, new ServoMotor(pin, min, max, startPosition)});
  servos[index]->initComponent(serialDebug);
  compDebug("register prop: " + String(servos[index]->name));
}

void ServoManager::registerServo(byte index, byte pin, int pulseMin, int pulseMax, int startPulse)
{
  if (!checkInit())
    return;
  
  if (!checkServoPin((int)pin))
    return;

  servos.insert({(int)index, new ServoMotor(pin, pulseMin, pulseMax, startPulse)});
  servos[index]->initComponent(serialDebug);

  compDebug("register prop: " + String(servos[index]->name));
}

void ServoManager::setServoPulseWidth(byte index, int value)
{
  if (servos.count(index) == 0)
  {
    compError("incorrect index: " + String(index));
    return;
  }
  compDebug("set servo#" + String(index) + " pulse width: " + String(value));
  servos[index]->writeMicroseconds(value);
}

void ServoManager::setServoRelativePosition(byte index, float value)
{
  if (servos.count(index) == 0)
  {
    compError("incorrect index: " + String(index));
    return;
  }
  compDebug("set servo#" + String(index) + " rel: " + String(value));
  servos[index]->goToPosition(value);
}

void ServoManager::setServoMin(byte index, float value)
{
  if (servos.count(index) == 0)
  {
    compError("incorrect index: " + String(index));
    return;
  }
  compDebug("set servo#" + String(index) + " min: " + String(value));
  servos[index]->setMin(value);
  servos[index]->goToPosition(value);
}

void ServoManager::setServoMax(byte index, float value)
{
  if (servos.count(index) == 0)
  {
    compError("incorrect index: " + String(index));
    return;
  }
  compDebug("set servo#" + String(index) + " max: " + String(value));
  servos[index]->setMax(value);
  servos[index]->goToPosition(value);
}

void ServoManager::setServoPulseMin(byte index, int value)
{
  if (servos.count(index) == 0)
  {
    compError("incorrect index: " + String(index));
    return;
  }
  compDebug("set servo#" + String(index) + " pulse min: " + String(value));
  servos[index]->setPulseMin(value);
  servos[index]->writeMicroseconds(value);
}

void ServoManager::setServoPulseMax(byte index, int value)
{
  if (servos.count(index) == 0)
  {
    compError("incorrect index: " + String(index));
    return;
  }
  compDebug("set servo#" + String(index) + " pulse max: " + String(value));
  servos[index]->setPulseMax(value);
  servos[index]->writeMicroseconds(value);
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
    if (checkCommandArguments(command, "if", true))
    {
      byte index = command.getInt(0);
      float value = command.getFloat(1);
      setServoRelativePosition(index, value);
      return true;
    }
  }
  if (address.equals("/servo/min"))
  {
    if (checkCommandArguments(command, "if", true))
    {
      byte index = command.getInt(0);
      float value = command.getInt(1);
      setServoMin(index, value);
      return true;
    }
  }
  if (address.equals("/servo/max"))
  {
    if (checkCommandArguments(command, "if", true))
    {
      byte index = command.getInt(0);
      float value = command.getInt(1);
      setServoMin(index, value);
      return true;
    }
  }
  return false;
}
#endif