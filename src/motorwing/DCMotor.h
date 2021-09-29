#pragma once
#include "../common/Component.h"
#include <Adafruit_MotorShield.h>

class DCMotor : public Component
{
public:
    DCMotor(Adafruit_DCMotor* motor, int port, int maxSpeed);

    void initComponent(bool serialDebug);
    void update();

    void run(float speed); // -1 1
    void setMaxSpeed(int value);
    void stop();

protected:
    int maxSpeed;
    Adafruit_DCMotor* motor;
};