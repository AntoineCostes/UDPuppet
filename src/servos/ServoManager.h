#pragma once
#include "../common/Manager.h"
#include "ServoMotor.h"

// TODO parameter for setting i2c address ?
// TODO parameter for setting PWM frequency ?

class ServoManager : public Manager
{

public:
  ServoManager();

  void initManager();

  void registerServo(int pin, int min, int max, int start, bool inverse, bool isMultiServo, bool useInSequences);

  bool handleCommand(OSCMessage &command) override;

  void servoGoToAbsolute(int index, int value);
  void servoGoTo(int index, float value);
  void servoGoToStart(int index);
  bool useInSequences(int index);

  void setServoMin(int index, int value);
  void setServoMax(int index, int value);
  void setServoMin(int index, float value);
  void setServoMax(int index, float value);
  
  void setServoInverse(int index, bool value);

  // void setMultiServoAbs(int index, int value);
  // void setMultiServoRel(int index, float value);


protected:
  bool isIndexValid(int index);
  std::vector<ServoMotor *> servos;
  Adafruit_PWMServoDriver* pwm;

};