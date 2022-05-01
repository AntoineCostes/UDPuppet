
#pragma once
#include "../common/Component.h"

#include <ESP32Servo.h>

class ServoMotor : public Component
{
public:
    ServoMotor(int pin, int min, int max, int start);

    void initComponent(bool serialDebug);
    void update();
    
    // void set(int value);
    void setAbs(int value);
    void setRel(float value);
    void setMin(int value);
    void setMax(int value);
    void setMin(float value);
    void setMax(float value);
    void setInverse(bool value);
    void setStart(int value);

    int getMin();
    int getMax();
    bool getInverse();

  // enum ServoMode
  // {
  //     PLAYING,
  //     READY,
  //     STREAMING
  // } mode;
  
protected:
    Servo servo;
    int pin;
};