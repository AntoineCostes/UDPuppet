#include "Config.h"
#include "src/common/PuppetMaster.h"
// dependencies:
// Adafruit_NeoPixel.h
// OSCMessage.h
// ESP32Servo.h
// ESPAsyncWebServer.h
// AsyncTCP
// ESPAsyncTCP
// ESPmDNS
// ESP8266mDNS
// ArduinoOTA

// TODO remove index
// TODO local libs

// RAPPEL après avoir uploadé il faut rebooter la carte manuellement

PuppetMaster master;
long lastLoopTime = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(3000); // wait for serial console

  master.initManager(); // TODO make singleton and rename ?

#ifdef CHANTDRIER
  master.servo.registerServo(0, 0, 0.0f, 1.0f, 0.5f); // index, pin, min, max, start
  master.servo.registerServo(1, 1, 0.0f, 0.5f, 0.1f); // index, pin, min, max, start
  master.servo.registerServo(2, 2, 0.0f, 1.0f, 0.5f); // index, pin, min, max, start
  master.servo.registerServo(3, 3, 0.0f, 1.0f, 0.5f); // index, pin, min, max, start
#endif

#ifdef CAMEMBERT
#ifdef ESP32
  master.servo.registerServo(0, 27, 0.5f, 0.7f, 1.0f); // index, pin, min, max, start
  master.servo.setServoInverse(0, true);
  // master.servo.registerServo(0, 27, 140, 80, 139); // index, pin, min, max, start
#else
  // master.servo.registerServo(0, 14, 140, 50, 139); // index, pin, min, max, start
#endif
#endif

#ifdef AMPOULE
  // couleur
  master.led.registerLedStrip(0, 21, 10, NEO_GRB + NEO_KHZ800); // index, pin, nbLeds, ledType
  // master.led.registerLedStrip(0, 21, 10, NEO_RGB + NEO_KHZ800); // index, pin, nbLeds, ledType
  // rotation tête
  master.servo.registerServo(0, 15, 0, 180, 90); // index, pin, min, max, start
  // master.servo.registerServo(0, 23, 0.0f, 1.0f, 0.5f); // index, pin, min, max, start
#endif

#ifdef BASE
  // led
  master.led.registerLedStrip(0, 21, 10, NEO_GRB + NEO_KHZ800);
  // pied
  master.servo.registerServo(0, 27, 50, 140, 70); // index, pin, min, max, start
  // cou
  master.servo.registerServo(1, 12, 60, 120, 120); // index, pin, min, max, start
  //rotation
  master.motorwing.registerStepper(0, 14, 15, 32, 13);
  master.motorwing.stepperSetMaxSpeed(0, 500.0f); // MAX VALUE WITH CHINESE DRIVER
  master.motorwing.stepperSetAccel(0, 250.0f);
  // TODO #define REVOLUTION_STEPS 1025 ?
#endif

#ifdef BOBINE
  master.led.registerLedStrip(0, 23, 10, NEO_GRB + NEO_KHZ800);
  master.motorwing.registerShieldv2Stepper(0, 200, MotorShield2Manager::StepperPort::M3_M4);
 //master.motorwing.registerStepper(0, 21, 17);
#endif

#ifdef CORBEILLE
  master.led.registerLedStrip(0, 21, 10, NEO_GRB + NEO_KHZ800);
  master.led.setBrightness(0.2f);
  master.motorwing.addDCMotor(MotorShield2Manager::DCPort::M1);
  master.motorwing.addDCMotor(MotorShield2Manager::DCPort::M2);
#endif

#ifdef ROOMBA
  master.led.setBrightness(0.3f);
  master.led.registerLedStrip(0, 12, 10, NEO_GRB + NEO_KHZ800);
  master.roomba.registerRoomba(33, 32, 15);
  master.roomba.setText(0, "Hello la cie !");
#endif
}

void loop()
{
  master.update();

  // ensure at least 1ms between frames
  if (millis() == lastLoopTime)
  {
    delay(1);
  }
  lastLoopTime = millis();
  
}
