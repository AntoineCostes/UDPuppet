#ifdef NUM_SERVOS
#include "ServoMotor.h"

ServoMotor::ServoMotor(int pin, int min, int max, int start, bool inverse, bool useInSequences, Adafruit_PWMServoDriver* pwm) :
                        Component("servo_" + String(pin)),
                        pin(pin),
                        min(min),
                        max(max),
                        start(start),
                        inverse(inverse),
                        pwm(pwm)
{
  if (min < 0 || min > 180) min = 0;
  if (max < 0 || max > 180) max = 180;
  if (start < 0 || start > 180) start = 90;
  Component::useInSequences = useInSequences;
}

void ServoMotor::update()
{
}

void ServoMotor::initComponent(bool serialDebug)
{
    if (!isMultiServo()) servo.attach(pin); // don't attach pin if using pwm servo driver
    Component::initComponent(serialDebug);
    goToStart();
}

void ServoMotor::goToRelative(float value)
{
  if (value < 0.0f || value > 1.0f)
  {
    compError("incorrect relative value: " + String(value));
    return;
  }
  compDebug("set relative value: "+String(value));
  goTo(min + value*(max - min));
}

void ServoMotor::goTo(int value)
{
  if (value < min || value > max)
  {
    compError("incorrect absolute value: " + String(value));
    return;
  }
  if (!initialized)
  {
    compError("servo was not initialized !");
    return;
  }

  if (inverse) value = max + min - value; 

  compDebug("set absolute value: "+String(value));

  if (isMultiServo()) pwm->setPWM(pin, 0, map(value, 0, 180, PWM_MIN, PWM_MAX));
  else servo.write(value);
}

void ServoMotor::goToStart()
{
  goTo(start);
}

void ServoMotor::setStart(int value)
{
  if (value < 0 || value > 180)
  {
    compError("can't set start, incorrect value: " + String(value));
    return;
  }
  compDebug("set start value: " + String(value));
  start = value;
}

void ServoMotor::setMin(int value)
{
  if (value < 0 || value >= max)
  {
    compError("can't set min, incorrect value: " + String(value));
    return;
  }
  compDebug("set min value: "+String(value));
  min = value;
  goTo(inverse?max:value);
}

void ServoMotor::setMax(int value)
{
  if (value < 0 || value <= min)
  {
    compError("can't set max, incorrect value: " + String(value));
    return;
  }
  compDebug("set max value: "+String(value));
  max = value;
  goTo(inverse?min:value);
}

void ServoMotor::setMin(float value)
{
  if (value < 0.0f || value > 1.0f)
  {
    compError("incorrect min relative value: " + String(value));
    return;
  }
  setMin(int(180*value));
}

void ServoMotor::setMax(float value)
{
  if (value < 0.0f || value > 1.0f)
  {
    compError("incorrect max relative value: " + String(value));
    return;
  }
  setMax(int(180*value));
}

void ServoMotor::setInverse(bool value)
{
  inverse = value;
}

int ServoMotor::getPin()
{
  return pin;
}

// int ServoMotor::getMin()
// {
//   return min;
// }

// int ServoMotor::getMax()
// {
//   return max;
// }

// bool ServoMotor::getInverse()
// {
//   return inverse;
// }

bool ServoMotor::isMultiServo()
{
  return pwm != nullptr;
}
#endif