
#pragma once
#include "../common/Component.h"

#ifdef HAS_SERVO
#include <ESP32Servo.h>

class ServoMotor : public Component
{
public:
    ServoMotor(byte pin, byte min, byte max, byte start);

    void initComponent(bool serialDebug) override;
    void update() override;
    
    // void set(int value);
    void setAbs(int value);
    void setRel(float value);
    void setMin(int value);
    void setMax(int value);
    void setStart();

  // enum ServoMode
  // {
  //     PLAYING,
  //     READY,
  //     STREAMING
  // } mode;
  
protected:
    Servo servo;
    int pin;
    int min;
    int max;
    int start;
};
#endif