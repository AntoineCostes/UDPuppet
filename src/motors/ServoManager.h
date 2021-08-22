#pragma once
#include "../common/Manager.h"
#include "ServoMotor.h"

class ServoManager : public Manager
{

public:
  ServoManager();

  void initManager();
  void update() override;

  void registerServo(byte index, byte pin, byte min, byte max, byte start);

  bool handleCommand(OSCMessage &command) override;

  // void setServo(int index, int value);
  void setServoAbs(int index, int value);
  void setServoRel(int index, float value);
  void setServoMin(int index, int value);
  void setServoMax(int index, int value);

protected:
  std::map<int, ServoMotor *> servos;
};