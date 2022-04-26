#include "ServoMotor.h"

ServoMotor::ServoMotor(int pin, int min, int max, int start) :
                        Component("servo_pin" + String(pin)),
                        pin(pin)
{
  intParameters["min"] = min;
  intParameters["max"] = max;
  intParameters["start"] = start;
  boolParameters["inverse"] = false;
}

void ServoMotor::initComponent(bool serialDebug)
{
    //pinMode(pin, OUTPUT);
    servo.attach(pin);
    Component::initComponent(serialDebug);
    setAbs(intParameters["start"]);
}

void ServoMotor::update()
{
  if (!checkInit()) return;

  // nothing
}

void ServoMotor::setAbs(int value)
{
  if (!checkInit()) return;

  if (value < intParameters["min"] || value > intParameters["max"])
  {
    compError("incorrect absolute value: " + String(value));
    return;
  }
  if (boolParameters["inverse"]) value = intParameters["max"] + intParameters["min"] - value; 
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
  compDebug("set rel"+String(value));
  setAbs(intParameters["min"] + value*(intParameters["max"] - intParameters["min"]));
}

void ServoMotor::setStart(int value)
{
  if (!checkInit()) return;
  
  if (value < 0 || value > 180)
  {
    compError("can't set start, incorrect value: " + String(value));
    return;
  }

  intParameters["start"] = value;
  overrideFlashParameters();
  setAbs(intParameters["start"]);
}

void ServoMotor::setMin(int value)
{
  if (!checkInit()) return;

  if (value < 0 || value >= intParameters["max"])
  {
    compError("can't set min, incorrect value: " + String(value));
    return;
  }
  intParameters["min"] = value;
  overrideFlashParameters();
  setRel(0.0f);
}

void ServoMotor::setMax(int value)
{
  if (!checkInit()) return;

  if (value < 0 || value <= intParameters["min"])
  {
    compError("can't set max, incorrect value: " + String(value));
    return;
  }
  intParameters["max"] = value;
  overrideFlashParameters();
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
  setAbs(value);
}

void ServoMotor::setInverse(bool value)
{
  if (!checkInit()) return;

  boolParameters["inverse"] = value;
  overrideFlashParameters();
}