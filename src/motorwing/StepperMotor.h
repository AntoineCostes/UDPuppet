
#pragma once
#include "../common/Component.h"

#ifdef HAS_MOTORWING
#include "../libs/AccelStepper/AccelStepper.h"

class StepperMotor : public Component
{
public:
  StepperMotor(byte id, AccelStepper *stepper);

  void initComponent(bool serialDebug) override;
  void update() override;

  enum StepperControlMode
  {
    POSITION,
    SPEED
  } mode;

  void goTo(long value);
  void moveTo(long value);
  void reset();
  void setSpeed(float value);
  void setSpeedRel(float value);
  void setAcceleration(float value);
  void setMaxSpeed(float value);
  long currentPosition();
  float currentSpeed();
  float maxSpeed();
  // float acceleration(); 

protected:
  AccelStepper* stepper;
};
#endif