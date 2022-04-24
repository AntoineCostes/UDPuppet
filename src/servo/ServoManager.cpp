// #include "ServoManager.h"

// ServoManager::ServoManager() : Manager("servo")
// {
//   serialDebug = SERVO_DEBUG;
// }

// void ServoManager::initManager()
// {
// #ifdef HAS_MULTISERVO
//   Manager::initManager();
//   pwm = Adafruit_PWMServoDriver();  // TODO set i2c address
//   pwm.begin();
//   pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
//   Component::registerPin(22); // SCL port
//   Component::registerPin(23); // SDA port
// #elif defined(HAS_SERVO)
//   Manager::initManager();
// #endif
// }

// void ServoManager::update()
// {
//   if (!checkInit())
//     return;
//   // nothing
// }

// bool ServoManager::checkServoPin(int pin)
// {
// #ifdef HAS_SERVO
//   std::set<int> servoPins = {2, 4, 12, 13, 14, 15, 16, 17, 21, 22, 23, 25, 32};
// #ifdef ESP32
//   if (!SERVO_ALLOW_UNSAFE_PINS)
//     if (servoPins.find(pin) == servoPins.end())
//     {
//       compError("pin " + String(pin) + " is not valid to attach Servo !");
//       return false;
//     }
// #endif

//   if (!Component::registerPin(pin))
//   {
//       compError("cannot register servo !");
//       return false;
//   } 
//   return true;
// #else
//   return false;
// #endif
// }

// void ServoManager::registerServo(int index, int pin, float startPosition)
// {
//   if (!checkInit())
//     return;
  
// #ifdef HAS_MULTISERVO
//   if (index < 0 || index > 7)
//   {
//     compError("incorrect index: " + String(index));
//     return;
//   }
//   servos.insert({index, new ServoMotor(pin, startPosition)});
//   servos[index]->Component::initComponent(serialDebug); // don't attach pin
//   setMultiServo(index, startPosition);
//   compDebug("register prop: " + String(servos[index]->name));

// #elif defined(HAS_SERVO)
//   if (!checkServoPin(pin))
//     return;

//   servos.insert({index, new ServoMotor(pin, startPosition)});
//   servos[index]->initComponent(serialDebug);
//   compDebug("register prop: " + String(servos[index]->name));
  
// #endif
// }

// void ServoManager::registerServo(int index, int pin, float min, float max, float startPosition)
// {
//   if (!checkInit())
//     return;
  
// #ifdef HAS_MULTISERVO
//   if (index < 0 || index > 7)
//   {
//     compError("incorrect index: " + String(index));
//     return;
//   }
//   servos.insert({index, new ServoMotor(pin, min, max, startPosition)});
//   servos[index]->Component::initComponent(serialDebug); // don't attach pin
//   setMultiServo(index, startPosition);
//   compDebug("register prop: " + String(servos[index]->name));

// #elif defined(HAS_SERVO)
//   if (!checkServoPin(pin))
//     return;

//   servos.insert({index, new ServoMotor(pin, min, max, startPosition)});
//   servos[index]->initComponent(serialDebug);
//   compDebug("register prop: " + String(servos[index]->name));
// #endif
// }

// void ServoManager::registerServo(int index, int pin, int pulseMin, int pulseMax, int startPulse)
// {
//   if (!checkInit())
//     return;
  
// #ifdef HAS_MULTISERVO
//   if (index < 0 || index > 7)
//   {
//     compError("incorrect index: " + String(index));
//     return;
//   }
//   servos.insert({index, new ServoMotor(pin, pulseMin, pulseMax, startPulse)});
//   servos[index]->Component::initComponent(serialDebug); // don't attach pin
//   setMultiServoPulse(index, startPulse);
//   compDebug("register prop: " + String(servos[index]->name));
  
// #elif defined(HAS_SERVO)
//   if (!checkServoPin(pin))
//     return;

//   servos.insert({index, new ServoMotor(pin, pulseMin, pulseMax, startPulse)});
//   servos[index]->initComponent(serialDebug);
//   compDebug("register prop: " + String(servos[index]->name));
  
// #endif
// }

// void ServoManager::setServoPulseWidth(int index, int value)
// {
//   if (servos.count(index) == 0)
//   {
//     compError("incorrect index: " + String(index));
//     return;
//   }
//   compDebug("set servo#" + String(index) + " pulse width: " + String(value));
  
// #ifdef HAS_MULTISERVO
//   setMultiServoPulse(index, value);
// #elif defined(HAS_SERVO)
//   servos[index]->writeMicroseconds(value);
// #endif
// }

// void ServoManager::setServoRelativePosition(int index, float value)
// {
//   if (servos.count(index) == 0)
//   {
//     compError("incorrect index: " + String(index));
//     return;
//   }
//   compDebug("set servo#" + String(index) + " rel: " + String(value));

// #ifdef HAS_MULTISERVO
//   setMultiServo(index, value);
// #elif defined(HAS_SERVO)
//   servos[index]->goToPosition(value);
// #endif
// }

// void ServoManager::setServoMin(int index, float value)
// {
//   if (servos.count(index) == 0)
//   {
//     compError("incorrect index: " + String(index));
//     return;
//   }
//   compDebug("set servo#" + String(index) + " min: " + String(value));
//   servos[index]->setMin(value);

// #ifdef HAS_MULTISERVO
//   setMultiServo(index, 0.0f);
// #elif defined(HAS_SERVO)
//   servos[index]->goToPosition(0.0f);
// #endif
// }

// void ServoManager::setServoMax(int index, float value)
// {
//   if (servos.count(index) == 0)
//   {
//     compError("incorrect index: " + String(index));
//     return;
//   }
//   compDebug("set servo#" + String(index) + " max: " + String(value));
//   servos[index]->setMax(value);

// #ifdef HAS_MULTISERVO
//   setMultiServo(index, 1.0f);
// #elif defined(HAS_SERVO)
//   servos[index]->goToPosition(1.0f);
// #endif
// }

// void ServoManager::setServoPulseMin(int index, int value)
// {
//   if (servos.count(index) == 0)
//   {
//     compError("incorrect index: " + String(index));
//     return;
//   }
//   compDebug("set servo#" + String(index) + " pulse min: " + String(value));
//   servos[index]->setPulseMin(value);

// #ifdef HAS_MULTISERVO
//   setMultiServoPulse(index, value);
// #elif defined(HAS_SERVO)
//   servos[index]->writeMicroseconds(value);
// #endif
// }

// void ServoManager::setServoPulseMax(int index, int value)
// {
//   if (servos.count(index) == 0)
//   {
//     compError("incorrect index: " + String(index));
//     return;
//   }
//   compDebug("set servo#" + String(index) + " pulse max: " + String(value));
//   servos[index]->setPulseMax(value);
  
// #ifdef HAS_MULTISERVO
//   setMultiServoPulse(index, value);
// #elif defined(HAS_SERVO)
//   servos[index]->writeMicroseconds(value);
// #endif
// }

// void ServoManager::setServoInverse(int index, bool value)
// {
//   if (servos.count(index) == 0)
//   {
//     compError("incorrect index: " + String(index));
//     return;
//   }
//   servos[index]->setInverse(value);
// }

// void ServoManager::setMultiServo(int index, float value)
// {
//   compDebug("set multiservo#" + String(index) + " value: " + String(value));
//   if (!Component::checkRange("multiservo pulse width", value, 0.0f, 1.0f)) return;

//   int pulseWidth = servos[index]->getPulseMin() + int(value*(servos[index]->getPulseMax() - servos[index]->getPulseMin()));
//   setMultiServoPulse(index, pulseWidth);
// }

// void ServoManager::setMultiServoPulse(int index, int pulseWidth)
// {
//   if (servos.count(index) == 0)
//   {
//     compError("incorrect index: " + String(index));
//     return;
//   }
// #ifdef HAS_MULTISERVO
//   compDebug("set multiservo#" + String(index) + " pulse width: " + String(pulseWidth));
//   if (!Component::checkRange("multiservo pulse width", pulseWidth, servos[index]->getPulseMin(), servos[index]->getPulseMax())) return;


//   if (servos[index]->getInverse())
//     pwm.setPWM(index, 0, map(pulseWidth, servos[index]->SERVO_PWM_MIN, servos[index]->SERVO_PWM_MAX, 574, 150));
//   else
//     pwm.setPWM(index, 0, map(pulseWidth, servos[index]->SERVO_PWM_MIN, servos[index]->SERVO_PWM_MAX, 150, 574));
//  #else
//  compError("multiservo is not defined");
//   #endif
// }

// bool ServoManager::handleCommand(OSCMessage &command)
// {
//   if (!checkInit())
//     return false;

//   char buf[32];
//   command.getAddress(buf);
//   String address = String(buf);
//   compLog("handle command: " + address);

//   if (address.equals("/servo"))
//   {
//     if (checkCommandArguments(command, "if", true))
//     {
//       int index = command.getInt(0);
//       float value = command.getFloat(1);
//       setServoRelativePosition(index, value);
//       return true;
//     }
//   }
//   if (address.equals("/servo/min"))
//   {
//     if (checkCommandArguments(command, "if", true))
//     {
//       int index = command.getInt(0);
//       float value = command.getFloat(1);
//       setServoMin(index, value);
//       return true;
//     }
//   }
//   if (address.equals("/servo/max"))
//   {
//     if (checkCommandArguments(command, "if", true))
//     {
//       int index = command.getInt(0);
//       float value = command.getFloat(1);
//       setServoMax(index, value);
//       return true;
//     }
//   }
//   if (address.equals("/servo/inverse"))
//   {
//     if (checkCommandArguments(command, "ii", true))
//     {
//       int index = command.getInt(0);
//       float value = command.getInt(1);
//       setServoInverse(index, value>0);
//       return true;
//     }
//   }
//   return false;
// }