#include "ServoMotor.h"

#ifdef NUM_SERVOS
///
/// ServoMotor constructor with default values: pulse width will range from 544us to 2500us.
///
ServoMotor::ServoMotor(byte pin, float startPosition) : Component("servo_pin" + String(pin))
{
  setPulseMin(SERVO_PWM_MIN);
  setPulseMax(SERVO_PWM_MAX);

  if (!Component::checkRange("startPosition", startPosition, 0.0f, 1.0f)) startPosition = 0.5f;
  setStartPosition(startPosition);

  intParameters["pin"] = pin;
}

///
/// ServoMotor constructor defining pulse width range with 0,1 values (percentage of 180°).
///
ServoMotor::ServoMotor(byte pin, float min, float max, float startPosition) : Component("servo_pin" + String(pin))
{
  if (!Component::checkRange("min", min, 0.0f, 1.0f)) min = 0.0f;
  setPulseMin(map(min, 0.0f, 1.0f, SERVO_PWM_MIN, SERVO_PWM_MAX));

  if (!Component::checkRange("max", max, 0.0f, 1.0f)) max = 1.0f;
  setPulseMax(map(max, 0.0f, 1.0f, SERVO_PWM_MIN, SERVO_PWM_MAX));

  if (!Component::checkRange("startPosition", startPosition, 0.0f, 1.0f)) startPosition = 0.5f;
  setStartPosition(startPosition);

  intParameters["pin"] = pin;
}

///
/// ServoMotor constructor defining pulse width range explicitly.
///
ServoMotor::ServoMotor(byte pin, int pulseMin, int pulseMax, int startPulse) : Component("servo_pin" + String(pin))
{
  if (!Component::checkRange("min PWM", pulseMin, SERVO_PWM_MIN, SERVO_PWM_MAX)) pulseMin = SERVO_PWM_MIN;
  setPulseMin(pulseMin);
  
  if (!Component::checkRange("max PWM", pulseMax, SERVO_PWM_MIN, SERVO_PWM_MAX)) pulseMin = SERVO_PWM_MAX;
  setPulseMax(pulseMax);

  if (!Component::checkRange("start PWM", startPulse, pulseMin, pulseMax)) startPulse = 0.5f*(pulseMin + pulseMax);
  setStartPosition(map(startPulse, pulseMin, pulseMax, 0.0f, 1.0f));

  intParameters["pin"] = pin;
}

///
/// Initialization: attach servo to designated pin and go to start position.
///
void ServoMotor::initComponent(bool serialDebug)
{
    servo.attach(intParameters["pin"], intParameters["pulseMin"], intParameters["pulseMax"]);
    Component::initComponent(serialDebug);
    goToStartPosition();
}

///
/// Update: nothing to do
///
void ServoMotor::update()
{
}

///
/// Go to start position (defined in the constructor).
///
void ServoMotor::goToStartPosition()
{
  goToPosition(floatParameters["startPosition"]);
}

///
/// Move servo to given relative position [0,1].
/// The position will be mapped to pulse width range (defined in constructor).
///
void ServoMotor::goToPosition(float relativeValue)
{
  if (!checkInit()) return;
  
  if (!Component::checkRange("servo position", relativeValue, 0.0f, 1.0f)) return;

  compDebug("go to pos: "+String(relativeValue));
  int value = map(relativeValue, 0.0f, 1.0f, 0, 180);
  compDebug(String(value));
  compDebug(String(int(relativeValue*180)));
  servo.write(int(relativeValue*180));
  
  // FIXME
  /*
  int usVal = map(relativeValue, 0.0f, 1.0f, intParameters["pulseMin"], SERVO_PWM_MAX);// intParameters["pulseMax"]);
  servo.writeMicroseconds(usVal);
  compDebug("go to pos: "+String(relativeValue));
  compDebug(String(intParameters["pulseMax"])); 
  compDebug("send pulse width: " + String(usVal) + "us for value: " + relativeValue);
*/
}

///
/// Move servo with given pulse width.
/// The value must respect the range defined in constructor.
///
void ServoMotor::writeMicroseconds(int pulseWidth)
{
  if (!checkInit()) return;

  if (!Component::checkRange("servo pulse width", pulseWidth, intParameters["pulseMin"], intParameters["pulseMax"])) return;
 
  servo.writeMicroseconds(pulseWidth);
  compDebug("write microseconds: "+String(pulseWidth));
}

///
/// Set minimum pulse width parameter [544, 2500].
/// This defines the lowest servo position.
///
void ServoMotor::setPulseMin(int value)
{
 if (!Component::checkRange("servo min pulse width", value, SERVO_PWM_MIN, SERVO_PWM_MAX)) return;
 
  intParameters["pulseMin"] = value;
  overrideFlashParameters();
}

///
/// Set maximum pulse width parameter [544, 2500].
/// This defines the highest servo position.
///
void ServoMotor::setPulseMax(int value)
{
 if (!Component::checkRange("servo max pulse width", value, SERVO_PWM_MIN, SERVO_PWM_MAX)) return;

  floatParameters["pulseMax"] = value;
  overrideFlashParameters();
}

///
/// Set lowest servo position [0, 1].
/// Minimum pulse width is set by mapping value to [SERVO_PWM_MIN, SERVO_PWM_MAX].
///
void ServoMotor::setMin(float value)
{
  setPulseMin(map(value, 0.0f, 1.0f, SERVO_PWM_MIN, SERVO_PWM_MAX));
}

///
/// Set highest servo position [0, 1].
/// The value is mapped to [SERVO_PWM_MIN, SERVO_PWM_MAX]
///
void ServoMotor::setMax(float value)
{
  setPulseMax(map(value, 0.0f, 1.0f, SERVO_PWM_MIN, SERVO_PWM_MAX));
}

///
/// Set start position parameter [0, 1].
///
void ServoMotor::setStartPosition(float relativeValue)
{
 if (!Component::checkRange("servo start position", relativeValue, 0.0f, 1.0f)) 
 {
  floatParameters["start"] = 0.5f;
 }
 else
 {
   floatParameters["start"] = relativeValue;
 }
  overrideFlashParameters();
}
#endif