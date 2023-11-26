#pragma once
#ifdef HAS_STEPPER_DRIVER
#include "../common/Manager.h"
#include "StepperMotor.h"
#include "../utils/EventBroadcaster.h"


class StepperEvent2
{
public:
    enum Type
    {
        MOVED
    } type;
    int index;
    long position;
    long speed;
    float maxSpeed;
    StepperEvent2(Type type, int index, long position, long speed, float maxSpeed) : type(type), index(index), position(position), speed(speed), maxSpeed(maxSpeed) {}
};

class StepperManager : public Manager,
                            public EventBroadcaster<StepperEvent2>
{

public:
  StepperManager();

  void initManager() override;
  void update() override;

  bool handleCommand(OSCMessage &command) override;

  void registerStepper(int pin1, int pin2, int pin3, int pin4);
  void registerStepper(int step, int dir);
  void registerStepper(int step, int dir, int enable);


  void stepperGoTo(int index, long value);
  void stepperGoToFromStart(int index, long value);
  void stepperMove(int index, long value);
  void stepperReset(int index);
  void stepperSetSpeed(int index, float value);
  void stepperSetSpeedRel(int index, float value);
  void stepperSetAccel(int index, float value);
  void stepperSetMaxSpeed(int index, float value);
  void stepperRelease(int index);
  
  
  std::vector<StepperMotor *> steppers;


protected:
  void registerStepper(int id, AccelStepper *stepper);
  void registerStepper(int id, AccelStepper *stepper, int enablePin);
  long lastEventTime;
  long lastEventPos;

};
#endif