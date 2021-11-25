#pragma once
#include "../common/Manager.h"
#include "ServoMotor.h"

#ifdef HAS_SERVO
class ServoManager : public Manager
{

public:
  ServoManager();

  void initManager();
  void update() override;

  void registerServo(byte index, byte pin, float startPosition);
  void registerServo(byte index, byte pin, float min, float max, float startPosition);
  void registerServo(byte index, byte pin, int pulseMin, int pulseMax, int startPulse);

  bool handleCommand(OSCMessage &command) override;

  void setServoPulseWidth(byte index, int value);
  void setServoRelativePosition(byte index, float value);
  void setServoMin(byte index, float value);
  void setServoMax(byte index, float value);
  void setServoPulseMin(byte index, int value);
  void setServoPulseMax(byte index, int value);
  
  bool checkServoPin(byte pin); // FIXME protected

protected:
  std::map<int, ServoMotor *> servos;
};
#endif