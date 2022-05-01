#include "ServoManager.h"

ServoManager::ServoManager() : Manager("servo")
{
  serialDebug = SERVO_DEBUG;
}

void ServoManager::initManager()
{
#ifdef HAS_MULTISERVO

  if (Component::forbiddenPins.find(22) != Component::forbiddenPins.end() || Component::forbiddenPins.find(23) != Component::forbiddenPins.end())
  {
    compError("Adafruit_PWMServoDriver needs SCL/SDA ports to be free !");
    return;
  }
  Manager::initManager();
  pwm = Adafruit_PWMServoDriver();  // TODO set i2c address
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
#elif defined(HAS_SERVO)
  Manager::initManager();
#endif
}

void ServoManager::update()
{
  if (!checkInit())
    return;
  // nothing
}

bool ServoManager::checkServoPin(int pin)
{
#ifdef HAS_SERVO
  std::set<int> servoPins = {2, 4, 12, 13, 14, 15, 16, 17, 21, 22, 23, 25, 32};
#ifdef ESP32
  if (!SERVO_ALLOW_UNSAFE_PINS)
    if (servoPins.find(pin) == servoPins.end())
    {
      compError("pin " + String(pin) + " is not valid to attach Servo !");
      return false;
    }
#endif

  if (!Component::registerPin(pin))
  {
      compError("cannot register servo !");
      return false;
  }
  return true;
#else
  return false;
#endif

}

void ServoManager::registerServo(int index, int pin, int min, int max, int start)
{
  if (!checkInit())
    return;
  
#ifdef HAS_MULTISERVO
  if (index < 0 || index > 7)
  {
    compError("incorrect index: " + String(index));
    return;
  }

  if (servos.count(index) != 0)
  {
    compError("servo was already registered on this pin");
    return;
  }
  servos.insert({index, new ServoMotor(pin, min, max, start)});
  servos[index]->initComponent(serialDebug); // don't attach pin
  compDebug("registered prop: " + String(servos[index]->name));
  setMultiServoAbs(index, start);
  
#elif defined(HAS_SERVO)
  if (!checkServoPin(pin))
    return;

  servos.insert({index, new ServoMotor(pin, min, max, start)});
  servos[index]->initComponent(serialDebug);
  compDebug("registered prop: " + String(servos[index]->name));
#endif
}

void ServoManager::setServoAbs(int index, int value)
{
  if (servos.count(index) == 0)
  {
    compError("incorrect index: " + String(index));
    return;
  }
  compDebug("set servo#" + String(index) + " abs: " + String(value));
  
#ifdef HAS_MULTISERVO
  setMultiServoAbs(index, value);
#elif defined(HAS_SERVO)
  servos[index]->setAbs(value);
#endif

}

void ServoManager::setServoRel(int index, float value)
{
  if (servos.count(index) == 0)
  {
    compError("incorrect index: " + String(index));
    return;
  }
  compDebug("set servo#" + String(index) + " rel: " + String(value));
  
#ifdef HAS_MULTISERVO
  setMultiServoRel(index, value);
#elif defined(HAS_SERVO)
  servos[index]->setRel(value);
#endif
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
  
#ifdef HAS_MULTISERVO
  setMultiServoRel(index, 0.0f);
#elif defined(HAS_SERVO)
#endif
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
  
#ifdef HAS_MULTISERVO
  setMultiServoRel(index, 1.0f);
#elif defined(HAS_SERVO)
#endif
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
  
#ifdef HAS_MULTISERVO
  setMultiServoRel(index, 0.0f);
#elif defined(HAS_SERVO)
#endif
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
  
#ifdef HAS_MULTISERVO
  setMultiServoRel(index, 1.0f);
#elif defined(HAS_SERVO)
#endif
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

void ServoManager::setMultiServoRel(int index, float value)
{
  compDebug("set multiservo#" + String(index) + " value: " + String(value));
  if (!Component::checkRange("multiservo rel", value, 0.0f, 1.0f)) return;

  int abs = servos[index]->getMin() + int(value*(servos[index]->getMax() - servos[index]->getMin()));
  setMultiServoAbs(index, abs);
}

void ServoManager::setMultiServoAbs(int index, int value)
{
  if (servos.count(index) == 0)
  {
    compError("incorrect index: " + String(index));
    return;
  }
#ifdef HAS_MULTISERVO
  compDebug("set multiservo#" + String(index) + ": " + String(value));
  if (!Component::checkRange("multiservo abs", value, servos[index]->getMin(), servos[index]->getMax())) return;

  if (servos[index]->getInverse())
    pwm.setPWM(index, 0, map(value, 0, 180, 574, 150));
  else
    pwm.setPWM(index, 0, map(value, 0, 180, 150, 574));
 #else
 compError("multiservo is not defined");
  #endif
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