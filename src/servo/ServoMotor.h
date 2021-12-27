
#pragma once
#include "../common/Component.h"

#ifdef NUM_SERVOS
#ifdef ESP32
#include <ESP32Servo.h>
#else
#include <Servo.h>
#endif
class ServoMotor : public Component
{
public:
    const int SERVO_PWM_MIN = 544; // RC servos are usually configure for 90° or 120° amplitude between 1000us-2000us
    const int SERVO_PWM_MAX = 2500; // 544us-2500us pusle width usually allows for full 180° amplitude

    ServoMotor(byte pin, float startPosition);
    ServoMotor(byte pin, float min, float max, float startPosition);
    ServoMotor(byte pin, int pulseMin, int pulseMax, int startPulse);

    void initComponent(bool serialDebug) override;
    void update() override;
    
    void goToStartPosition();
    void goToPosition(float relativeValue);
    void writeMicroseconds(int pulseWidth);
    void setPulseMin(int value);
    void setPulseMax(int value);
    void setMin(float value);
    void setMax(float value);
    void setStartPosition(float relativeValue);

  // enum ServoMode
  // {
  //     PLAYING,
  //     READY,
  //     STREAMING
  // } mode;
  
protected:
    Servo servo;
};
#endif