#pragma once
#include "../common/Component.h"

#ifdef HAS_MOTORWING
#include <Adafruit_MotorShield.h>

class DCMotor : public Component
{
public:
    DCMotor(Adafruit_DCMotor* motor, int port, int maxSpeed);

    void initComponent(bool serialDebug) override;
    void update() override;

    void run(float speed); // -1 1
    void setMaxSpeed(int value);
    void stop();

protected:
    int maxSpeed;
    Adafruit_DCMotor* motor;
};
#endif