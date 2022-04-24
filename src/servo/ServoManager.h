// #pragma once
// #include "../common/Manager.h"
// #include "ServoMotor.h"

// #ifdef HAS_MULTISERVO
// #include <Adafruit_PWMServoDriver.h>
// #endif

// class ServoManager : public Manager
// {

// public:
//   ServoManager();

//   void initManager();
//   void update() override;

//   void registerServo(int index, int pin, float startPosition);
//   void registerServo(int index, int pin, float min, float max, float startPosition);
//   void registerServo(int index, int pin, int pulseMin, int pulseMax, int startPulse);

//   bool handleCommand(OSCMessage &command) override;

//   void setServoPulseWidth(int index, int value);
//   void setServoRelativePosition(int index, float value);
//   void setServoMin(int index, float value);
//   void setServoMax(int index, float value);
//   void setServoPulseMin(int index, int value);
//   void setServoPulseMax(int index, int value);
//   void setServoInverse(int index, bool value);
  
//   void setMultiServo(int index, float value);
//   void setMultiServoPulse(int index, int pulseWidth);
  
//   bool checkServoPin(int pin); // FIXME protected

// protected:
//   std::map<int, ServoMotor *> servos;
// #ifdef HAS_MULTISERVO
// Adafruit_PWMServoDriver pwm;
// #endif
// };