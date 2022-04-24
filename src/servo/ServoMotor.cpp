// #include "ServoMotor.h"

// ///
// /// ServoMotor constructor with default values: pulse width will range from 544us to 2500us.
// ///
// ServoMotor::ServoMotor(int pin, float startPosition) : Component("servo_pin" + String(pin))
// {
//   intParameters["pulseMin"] = SERVO_PWM_MIN;
//   intParameters["pulseMax"] = SERVO_PWM_MAX;

//   if (!Component::checkRange("startPosition", startPosition, 0.0f, 1.0f)) startPosition = 0.5f;
//   setStartPosition(startPosition);

//   intParameters["pin"] = pin;
//   boolParameters["inverse"] = false;
// }

// ///
// /// ServoMotor constructor defining pulse width range with 0,1 values (percentage of 180°).
// ///
// ServoMotor::ServoMotor(int pin, float min, float max, float startPosition) : Component("servo_pin" + String(pin))
// {
//   intParameters["pulseMin"] = SERVO_PWM_MIN;
//   intParameters["pulseMax"] = SERVO_PWM_MAX;

//   if (!Component::checkRange("min", min, 0.0f, 1.0f)) min = 0.0f;
//   setPulseMin(min*(SERVO_PWM_MAX - SERVO_PWM_MIN) + SERVO_PWM_MIN);

//   if (!Component::checkRange("max", max, 0.0f, 1.0f)) max = 1.0f;
//   setPulseMax(max*(SERVO_PWM_MAX - SERVO_PWM_MIN) + SERVO_PWM_MIN);

//   if (!Component::checkRange("startPosition", startPosition, 0.0f, 1.0f)) startPosition = 0.5f;
//   setStartPosition(startPosition);

//   intParameters["pin"] = pin;
//   boolParameters["inverse"] = false;
// }

// ///
// /// ServoMotor constructor defining pulse width range explicitly.
// ///
// ServoMotor::ServoMotor(int pin, int pulseMin, int pulseMax, int startPulse) : Component("servo_pin" + String(pin))
// {
//   intParameters["pulseMin"] = SERVO_PWM_MIN;
//   intParameters["pulseMax"] = SERVO_PWM_MAX;
  
//   if (!Component::checkRange("min PWM", pulseMin, SERVO_PWM_MIN, SERVO_PWM_MAX)) pulseMin = SERVO_PWM_MIN;
//   setPulseMin(pulseMin);
  
//   if (!Component::checkRange("max PWM", pulseMax, SERVO_PWM_MIN, SERVO_PWM_MAX)) pulseMax = SERVO_PWM_MAX;
//   setPulseMax(pulseMax);

//   if (!Component::checkRange("start PWM", startPulse, pulseMin, pulseMax)) startPulse = 0.5f*(pulseMin + pulseMax);
//   setStartPosition((startPulse - pulseMin)/(pulseMax - pulseMin));

//   intParameters["pin"] = pin;
//   boolParameters["inverse"] = false;
// }

// ///
// /// Initialization: attach servo to designated pin and go to start position.
// ///
// void ServoMotor::initComponent(bool serialDebug)
// {
//     if (servo.attach(intParameters["pin"], intParameters["pulseMin"], intParameters["pulseMax"]))
//     {
//     Component::initComponent(serialDebug);
//     goToStartPosition();
//     }
// }

// ///
// /// Update: nothing to do
// ///
// void ServoMotor::update()
// {
// }

// ///
// /// Go to start position (defined in the constructor).
// ///
// void ServoMotor::goToStartPosition()
// {
//   goToPosition(floatParameters["start"]);
// }

// ///
// /// Move servo to given relative position [0,1].
// /// The position will be mapped to pulse width range (defined in constructor).
// ///
// void ServoMotor::goToPosition(float relativeValue)
// {
//   if (!checkInit()) return;
  
//   if (!Component::checkRange("servo position", relativeValue, 0.0f, 1.0f)) return;

//   if (boolParameters["inverse"]) relativeValue = 1.0f - relativeValue;
//   int usVal = int(relativeValue*(intParameters["pulseMax"] - intParameters["pulseMin"])) + intParameters["pulseMin"];
//   servo.writeMicroseconds(usVal);
//   compDebug("send pulse width: " + String(usVal) + "us for value: " + String(relativeValue));
// }

// ///
// /// Move servo with given pulse width.
// /// The value must respect the range defined in constructor.
// ///
// void ServoMotor::writeMicroseconds(int pulseWidth)
// {
//   if (!checkInit()) return;

//   if (!Component::checkRange("servo pulse width", pulseWidth, intParameters["pulseMin"], intParameters["pulseMax"])) return;
  
//   if (boolParameters["inverse"]) pulseWidth = intParameters["pulseMin"] + intParameters["pulseMax"] - pulseWidth;
//   // {
//   //   float relative = float(pulseWidth - intParameters["pulseMin"])/float(intParameters["pulseMax"] - intParameters["pulseMin"]);
//   // pulseWidth = intParameters["pulseMin"] + (intParameters["pulseMax"] - intParameters["pulseMin"])*(1.0f - relative;
//   // } 
//   servo.writeMicroseconds(pulseWidth);
//   compDebug("write microseconds: "+String(pulseWidth));
// }

// ///
// /// Set minimum pulse width parameter [544, 2500].
// /// This defines the lowest servo position.
// ///
// void ServoMotor::setPulseMin(int value)
// {
//  if (!Component::checkRange("servo min pulse width", value, SERVO_PWM_MIN, SERVO_PWM_MAX)) return;
//   if (value >= intParameters["pulseMax"])
//   {
//     compError("min cannot be greater than max !");
//     return;
//   }
 
//   intParameters["pulseMin"] = value;
//   overrideFlashParameters();
// }

// ///
// /// Set maximum pulse width parameter [544, 2500].
// /// This defines the highest servo position.
// ///
// void ServoMotor::setPulseMax(int value)
// {
//  if (!Component::checkRange("servo max pulse width", value, SERVO_PWM_MIN, SERVO_PWM_MAX)) return;
//   if (value <= intParameters["pulseMin"]) 
//   {
//     compError("max cannot be smaller than min !");
//     return;
//   }
//   intParameters["pulseMax"] = value;
//   overrideFlashParameters();
// }

// ///
// /// Set lowest servo position [0, 1].
// /// Minimum pulse width is set by mapping value to [SERVO_PWM_MIN, SERVO_PWM_MAX].
// ///
// void ServoMotor::setMin(float value)
// {
//  if (!Component::checkRange("servo min", value, 0.0f, 1.0f)) return;

//   setPulseMin(SERVO_PWM_MIN + int(value*(SERVO_PWM_MAX - SERVO_PWM_MIN)));
// }

// ///
// /// Set highest servo position [0, 1].
// /// The value is mapped to [SERVO_PWM_MIN, SERVO_PWM_MAX]
// ///
// void ServoMotor::setMax(float value)
// {
//  if (!Component::checkRange("servo max", value, 0.0f, 1.0f)) return;

//   setPulseMax(SERVO_PWM_MIN + int(value*(SERVO_PWM_MAX - SERVO_PWM_MIN)));
// }

// ///
// /// Set start position parameter [0, 1].
// ///
// void ServoMotor::setStartPosition(float relativeValue)
// {
//  if (!Component::checkRange("servo start position", relativeValue, 0.0f, 1.0f)) 
//  {
//   floatParameters["start"] = 0.5f;
//  }
//  else
//  {
//    floatParameters["start"] = relativeValue;
//  }
//   overrideFlashParameters();
// }

// ///
// /// Set inverse parameter.
// ///
// void ServoMotor::setInverse(bool value)
// {
//   boolParameters["inverse"] = value;
// }

// int ServoMotor::getPulseMin()
// {
//   return intParameters["pulseMin"];
// }

// int ServoMotor::getPulseMax()
// {
//   return intParameters["pulseMax"];
// }

// bool ServoMotor::getInverse()
// {
//   return boolParameters["inverse"];
// }
