// #ifdef NUM_SERVOS
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
    
  compDebug("registering servo on pin " + String(pin));

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
    std::set<int> recommendedPins;
    switch (BOARD_TYPE)
    {
      case HUZZAH32:
        recommendedPins = {2, 4, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33};
        if (recommendedPins.find(pin) == recommendedPins.end())
        {
          compError("Recommended pins to attach Servo on a ESP32 are : 2, 4, 12-19, 21-23, 25-27, 32-33");
#ifndef ALLOW_ESP32_SERVO_UNRECOMMENDED_PINS
          return;
#endif
        }
        break;

        case HUZZAH32_S3:
        recommendedPins = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 10, 21, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45,47};
        if (recommendedPins.find(pin) == recommendedPins.end())
        {
          compError("Recommended pins to attach Servo on a ESP32-S3 are : 1-21,35-45,47");
#ifndef ALLOW_ESP32_SERVO_UNRECOMMENDED_PINS
          return;
#endif
        }
        break;

        case XIAO_C3:
        recommendedPins = {1, 2, 3, 4, 5, 6, 7, 9, 10, 18, 19, 20, 21};
        if (recommendedPins.find(pin) == recommendedPins.end())
        {
          compError("Recommended pins to attach Servo on a ESP32-C3 are : 1-7, 9-10, 18-21");
#ifndef ALLOW_ESP32_SERVO_UNRECOMMENDED_PINS
          return;
#endif
        }
        break;
        
        case HUZZAH8266:
        case THINGESP8266:
        break;

        default:
          err("Unknwon board type !");
          break;
    }

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
// #endif