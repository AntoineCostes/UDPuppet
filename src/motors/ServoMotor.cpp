#include "ServoMotor.h"

ServoMotor::ServoMotor(int pin, int min, int max, int start, bool inverse) :
                        Component("servo_" + String(pin)),
                        pin(pin),
                        min(min),
                        max(max),
                        start(start)
{
  if (min < 0 || min > 180) min = 0;
  if (max < 0 || max > 180) max = 180;
  if (start < 0 || start > 180) start = 90;
  boolParameters["inverse"] = inverse;
  // overrideFlashParameters();
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

void ServoMotor::setAbs(int value)
{
  if (!checkInit()) return;

  if (value < min || value > max)
  {
    compError("incorrect absolute value: " + String(value));
    return;
  }
  if (boolParameters["inverse"]) value = max + min - value; 
  servo.write(value);
  compDebug("set absolute value"+String(value));
}

void ServoMotor::setRel(float value)
{
  if (!checkInit()) return;
  
  if (value < 0.0f || value > 1.0f)
  {
    compError("incorrect relative value: " + String(value));
    return;
  }
  compDebug("set relative value: "+String(value));
  setAbs(min + value*(max - min));
}

void ServoMotor::setStart(int value)
{
  if (!checkInit()) return;
  
  if (value < 0 || value > 180)
  {
    compError("can't set start, incorrect value: " + String(value));
    return;
  }

  start = value;
  setAbs(start);
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
  setRel(0.0f);
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
  setRel(1.0f);
}

void ServoMotor::setMin(float value)
{
  if (!checkInit()) return;

  if (value < 0.0f || value > 1.0f)
  {
    compError("incorrect min relative value: " + String(value));
    return;
  }

  setMin(int(180*value));
  setAbs(0);
}

void ServoMotor::setMax(float value)
{
  if (!checkInit()) return;

  if (value < 0.0f || value > 1.0f)
  {
    compError("incorrect max relative value: " + String(value));
    return;
  }

  setMax(int(180*value));
  setAbs(1.0f);
}

void ServoMotor::setInverse(bool value)
{
  if (!checkInit()) return;

  boolParameters["inverse"] = value;
  overrideFlashParameters();
}

int ServoMotor::getMin()
{
  return min;
}

int ServoMotor::getMax()
{
  return max;
}

bool ServoMotor::getInverse()
{
  return boolParameters["inverse"];
}