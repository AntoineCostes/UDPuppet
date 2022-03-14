#pragma once
#include "../common/Manager.h"
#include "StepperMotor.h"
#include "DCMotor.h"
#include "../utils/EventBroadcaster.h"

#ifdef HAS_MOTORWING
#include <Adafruit_MotorShield.h>

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
  void registerShieldv2Stepper(byte index, int steps, StepperPort port);
  void registerStepper(byte index, byte pin1, byte pin2, byte pin3, byte pin4);
  void stepperGoTo(byte index, long value);
  void stepperGoToFromStart(byte index, long value);
  void stepperMove(byte index, long value);
  void stepperReset(byte index);
  void stepperSetSpeed(byte index, float value);
  void stepperSetSpeedRel(byte index, float value);
  void stepperSetAccel(byte index, float value);
  void stepperSetMaxSpeed(byte index, float value);
  
  void addDCMotor( DCPort port);
  void dcRun(DCPort port, float value); // -1 1
  void dcStop(DCPort port);
  void dcStopAll();
  void dcMaxSpeed(DCPort port, int value);

protected:
  Adafruit_MotorShield AFMS;
  std::set<DCPort> usedPorts;
  void registerStepper(byte index, byte id, AccelStepper *stepper);
  long lastEventTime;
  long lastEventPos;

  std::map<byte, StepperMotor *> steppers;
  // std::map<byte, Adafruit_DCMotor *> dc;
  std::map<DCPort, DCMotor*> dcMotors;

private:
  Adafruit_StepperMotor *shieldStepper1;
  void forward1();
  void backward1();
  Adafruit_StepperMotor *shieldStepper2;
  void forward2();
  void backward2();
};
#endif