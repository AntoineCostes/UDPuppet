
#pragma once
#include "../common/Component.h"

#include <ESP32Servo.h>

class ServoMotor : public Component
{
public:
    ServoMotor(byte pin, byte min, byte max, byte start);

    void initComponent(bool serialDebug);
    void update();
    
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
