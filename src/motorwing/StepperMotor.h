
#pragma once
#include "../common/Component.h"

#include <AccelStepper.h>

class StepperMotor : public Component
{
public:
  StepperMotor(byte id, AccelStepper *stepper);

  void initComponent(bool serialDebug);
  void update();

  enum StepperControlMode
  {
    POSITION,
    SPEED
  } mode;

  float acceleration;
  float maxSpeed;

  void goTo(long value);
  void goToFromStart(long value);
  void moveTo(long value);
  void reset();
  void setSpeed(float value);
  void setSpeedRel(float value);
  void setAcceleration(float value);
  void setMaxSpeed(float value);
  long currentPosition();
  float currentSpeed();

protected:
  AccelStepper* stepper;
  long startValue;
};
