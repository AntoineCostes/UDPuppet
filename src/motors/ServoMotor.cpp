#include "ServoMotor.h"

#ifdef HAS_SERVO
ServoMotor::ServoMotor(byte pin, byte min, byte max, byte start) :
                        Component("servo_pin" + String(pin)),
                        pin(pin),
                        min(min),
                        max(max),
                        start(start)
{
  intParameters["min"] = (int)min;
  intParameters["max"] = (int)max;
  intParameters["start"] = (int)start;
}

void ServoMotor::initComponent(bool serialDebug)
{
    //pinMode(pin, OUTPUT);
    servo.attach(pin);
    Component::initComponent(serialDebug);
    setAbs(start);
}

void ServoMotor::update()
{
  if (!checkInit()) return;

  // nothing
}

// void ServoMotor::set(int value)
// {
//   if (!checkInit()) return;

//   if (value < min || value > max)
//   {
//     compError("incorrect absolute value: " + String(value));
//     return;
//   }
//   servo.write(value);
// }

void ServoMotor::setAbs(int value)
{
  if (!checkInit()) return;

  if (value < min || value > max)
  {
    compError("incorrect absolute value: " + String(value));
    return;
  }
  servo.write(value);
  compDebug("set "+String(value));
}

void ServoMotor::setRel(float value)
{
  if (!checkInit()) return;
  
  if (value < 0.0f || value > 1.0f)
  {
    compError("incorrect relative value: " + String(value));
    return;
  }
  int absVal = map(value * SERVO_MAX, 0, SERVO_MAX, min, max);
  servo.write(absVal);
  compDebug("set "+String(absVal));
}

void ServoMotor::setStart()
{
  if (!checkInit()) return;
  servo.write(start);
  compDebug("set start");
}

void ServoMotor::setMin(int value)
{
  if (!checkInit()) return;

  if (value < 0 || value >= max)
  {
    compError("can't set min, incorrect value: " + String(value));
    return;
  }
  min = value;
  intParameters["min"] = value;
  overrideFlashParameters();
}

void ServoMotor::setMax(int value)
{
  if (!checkInit()) return;

  if (value < 0 || value <= min)
  {
    compError("can't set max, incorrect value: " + String(value));
    return;
  }
  max = value;
  intParameters["max"] = value;
  overrideFlashParameters();
}
#endif