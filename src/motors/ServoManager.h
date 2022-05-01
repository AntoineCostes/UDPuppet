#pragma once
#include "../common/Manager.h"
#include "ServoMotor.h"

#ifdef HAS_MULTISERVO
#include <Adafruit_PWMServoDriver.h>
#endif

class ServoManager : public Manager
{

public:
  ServoManager();

  void initManager();
  void update() override;

  void registerServo(int index, int pin, int min, int max, int start);

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
  std::map<int, ServoMotor *> servos;
#ifdef HAS_MULTISERVO
Adafruit_PWMServoDriver pwm;
#endif

};