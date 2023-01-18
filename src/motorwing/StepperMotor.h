
#pragma once
#include "../common/Component.h"

#if defined(HAS_MOTORWING) || defined(HAS_STEPPER_DRIVER)
#include <AccelStepper.h>

class StepperMotor : public Component
{
public:
  StepperMotor(int id, AccelStepper *stepper);
  StepperMotor(int id, AccelStepper *stepper, int enablePin);

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
  void setEnablePin(int pin);
  void enableOutputs();
  long currentPosition();
  float currentSpeed();
  float maxSpeed();
  // float acceleration(); 

protected:
  AccelStepper* stepper;
};
#endif