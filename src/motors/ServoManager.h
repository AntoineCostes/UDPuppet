#pragma once
#include "../common/Manager.h"
#include "ServoMotor.h"

class ServoManager : public Manager
{

public:
  ServoManager();

  void initManager();
  void update() override;

  void registerServo(int index, int pin, int min, int max, int start);

  bool handleCommand(OSCMessage &command) override;

  // void setServo(int index, int value);
  void setServoAbs(int index, int value);
  void setServoRel(int index, float value);
  void setServoMin(int index, int value);
  void setServoMax(int index, int value);
  void setServoMin(int index, float value);
  void setServoMax(int index, float value);
  void setServoInverse(int index, bool value);

protected:
  std::map<int, ServoMotor *> servos;
};