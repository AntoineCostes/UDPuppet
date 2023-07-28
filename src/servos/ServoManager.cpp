#include "ServoManager.h"

ServoManager::ServoManager() : Manager("servo")
{
  serialDebug = SERVO_DEBUG;
}

void ServoManager::initManager()
{
  if (Component::forbiddenPins.find(22) != Component::forbiddenPins.end() || Component::forbiddenPins.find(23) != Component::forbiddenPins.end())
  {
    compError("Adafruit_PWMServoDriver needs SCL/SDA ports to be free !");
    return;
  }
  pwm = new Adafruit_PWMServoDriver();  
  pwm->begin();
  pwm->setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  Manager::initManager();
}


bool ServoManager::isIndexValid(int index)
{
  if (index < 0 || index >= servos.size())
  {
    compError("incorrect index: " + String(index));
    return false;
  }
  return true;
}

void ServoManager::registerServo(int pin, int min, int max, int start, bool inverse, bool isMultiServo, bool useInSequences)
{
  if (!checkInit())
    return;
    
  for (auto const &servo : servos)
    if (servo->getPin() == pin)
      if ( ( isMultiServo && servo->isMultiServo() ) || ( !isMultiServo && !servo->isMultiServo() ) )
      {
      compError("another servo was already registered on this pin");
      return;
      }

  if (isMultiServo)
  {
    if (pin < 0 || pin > 7)
    {
      compError("multiservo pin must be between 0 and 7");
      return;
    }
  } else
  {
#ifdef ESP32
    std::set<int> unsafePins = {2, 4, 12, 13, 14, 15, 16, 17, 21, 22, 23, 25, 32};
      if (unsafePins.find(pin) != unsafePins.end())
      {
        compError("pin " + String(pin) + " is unsafe for ESP32 to attach Servo !");
#ifndef SERVO_ALLOW_UNSAFE_PINS
        return;
#endif
      }
#endif

    if (!Component::registerPin(pin))
    {
        compError("cannot register servo : pin #"  +String(pin) + " is already used!");
        return;
    }
  }
  servos.emplace_back(new ServoMotor(pin, min, max, start, inverse, useInSequences, isMultiServo?pwm:nullptr));
  servos.back()->initComponent(serialDebug);
  compLog("registered servo: " + String(servos.back()->name));
}

void ServoManager::servoGoToAbsolute(int index, int value)
{
  if (!isIndexValid(index)) return;
  compDebug("servo#" + String(index) + " go to (abs): " + String(value));
  servos[index]->goTo(value);
}

void ServoManager::servoGoTo(int index, float value)
{
  if (!isIndexValid(index)) return;
  compDebug("servo#" + String(index) + " go to (rel): " + String(value));
  servos[index]->goToRelative(value);
}

void ServoManager::servoGoToStart(int index)
{
  if (!isIndexValid(index)) return;
  compDebug("servo#" + String(index) + " go to start");
  servos[index]->goToStart();
}

bool ServoManager::useInSequences(int index)
{
  return servos[index]->useInSequences;
}

void ServoManager::setServoMin(int index, int value)
{
  if (!isIndexValid(index)) return;
  compDebug("set servo#" + String(index) + " min: " + String(value));
  servos[index]->setMin(value);
}

void ServoManager::setServoMax(int index, int value)
{
  if (!isIndexValid(index)) return;
  compDebug("set servo#" + String(index) + " max: " + String(value));
  servos[index]->setMax(value);
}

void ServoManager::setServoMin(int index, float value)
{
  if (!isIndexValid(index)) return;
  compDebug("set servo#" + String(index) + " min: " + String(value));
  servos[index]->setMin(value);
}

void ServoManager::setServoMax(int index, float value)
{
  if (!isIndexValid(index)) return;
  compDebug("set servo#" + String(index) + " max: " + String(value));
  servos[index]->setMax(value);
}

void ServoManager::setServoInverse(int index, bool value)
{
  if (!isIndexValid(index)) return;
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
  compDebug("handle command: " + address);

  if (address.equals("/servo"))
  {
    if (checkCommandArguments(command, "ii", false))
    {
      int index = command.getInt(0);
      int value = command.getInt(1);
      servoGoToAbsolute(index, value);
      return true;
    }
    else if (checkCommandArguments(command, "if", false))
    {
      int index = command.getInt(0);
      float value = command.getFloat(1);
      servoGoTo(index, value);
      return true;
    }
    else if (checkCommandArguments(command, "f", true))
    {
      float value = command.getFloat(0);
      servoGoTo(0, value);
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
      setServoMax(index, value);
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