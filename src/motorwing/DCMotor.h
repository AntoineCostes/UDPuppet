#pragma once
#include "../common/Component.h"

#ifdef HAS_MOTORWING
#include "../libs/Adafruit_Motor_Shield_V2_Library/Adafruit_MotorShield.h"
// TODO include Adafruit_BuSIO

class DCMotor : public Component
{
public:
    DCMotor(Adafruit_DCMotor* motor, int port, int maxSpeed);

    void initComponent(bool serialDebug) override;
    void update() override;

    void run(float speed); // -1 1
    void setMaxSpeed(int value);
    int getMaxSpeed();
    void stop();

protected:
    int maxSpeed;
    Adafruit_DCMotor* motor;
};
#endif