#pragma once
#include "../common/Manager.h"
#include "ServoMotor.h"

class ServoManager : public Manager
{

public:
  ServoManager();

  void initManager();
  void update() override;

  void registerServo(int pin, int min, int max, int start, bool inverse);

  bool handleCommand(OSCMessage &command) override;

  void setServoAbs(int index, int value);
  void setServoRel(int index, float value);

  void setServoMin(int index, int value);
  void setServoMax(int index, int value);
  void setServoMin(int index, float value);
  void setServoMax(int index, float value);
  
  void setServoInverse(int index, bool value);

  void setMultiServoAbs(int index, int value);
  void setMultiServoRel(int index, float value);


protected:
  bool checkServoPin(int pin); 
  std::vector<ServoMotor *> servos;
  Adafruit_PWMServoDriver pwm;

};