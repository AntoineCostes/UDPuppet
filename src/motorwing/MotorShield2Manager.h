#pragma once
#include "../common/Manager.h"
#include "StepperMotor.h"
#include "DCMotor.h"
#include "../utils/EventBroadcaster.h"

#ifdef HAS_MOTORWING

class StepperEvent
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
    StepperEvent(Type type, int index, long position, long speed, float maxSpeed) : type(type), index(index), position(position), speed(speed), maxSpeed(maxSpeed) {}
};

class MotorShield2Manager : public Manager,
                            public EventBroadcaster<StepperEvent>
{

public:
  enum DCPort
  {
    M1 = 1,
    M2 = 2,
    M3 = 3,
    M4 = 4
  };
  enum StepperPort
  {
    M1_M2 = 1,
    M3_M4 = 2
  };
  MotorShield2Manager();

  void initManager() override;
  void update() override;

  bool handleCommand(OSCMessage &command) override;

  // TODO handle I2C address, step type
  void registerShieldv2Stepper(int steps, StepperPort port);
  void registerStepper(int pin1, int pin2, int pin3, int pin4);
  void registerStepper(int step, int dir);
  void stepperGoTo(int index, long value);
  void stepperGoToFromStart(int index, long value);
  void stepperMove(int index, long value);
  void stepperReset(int index);
  void stepperSetSpeed(int index, float value);
  void stepperSetSpeedRel(int index, float value);
  void stepperSetAccel(int index, float value);
  void stepperSetMaxSpeed(int index, float value);
  void stepperRelease(int index);
  
  void addDCMotor( DCPort port);
  void dcRun(DCPort port, float value); // -1 1
  void dcStop(DCPort port);
  void dcStopAll();
  void dcMaxSpeed(DCPort port, int value);
  std::map<DCPort, DCMotor*> dcMotors;
  std::vector<StepperMotor *> steppers;


protected:
  Adafruit_MotorShield AFMS;
  std::set<DCPort> usedPorts;
  void registerStepper(int id, AccelStepper *stepper);
  long lastEventTime;
  long lastEventPos;

  // std::map<int, Adafruit_DCMotor *> dc;

private:
  Adafruit_StepperMotor *shieldStepper1;
  void forward1();
  void backward1();
  Adafruit_StepperMotor *shieldStepper2;
  void forward2();
  void backward2();
};
#endif