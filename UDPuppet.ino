#include "Config.h"
#include "src/common/PuppetMaster.h"
// dependencies:
// OSC by Adrian Freed
// ESP32Servo
// Adafruit_BuSIO & Adafruit_Motor_Shield_V2_Library
// ESPAsyncWebServer https://github.com/me-no-dev/ESPAsyncWebServer
// AsyncTCP https://github.com/me-no-dev/AsyncTCP
// ESPAsyncTCP https://github.com/me-no-dev/ESPAsyncTCP

// TODO remove led index
// TODO local libs
// TODO ERROR mode if prop was not registered successfully
// TODO bluehouse config
// DELETE sequence button
// TODO steppermanager
// TODO checkComponents + ParameterEvent

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
  master.servo.registerServo(0, 10, 110, 100); // pin, min, max, start
  master.servo.setServoInverse(0, true);
  master.servo.setServoRel(0, 0.01f);
  master.servo.registerServo(1, 5, 110, 100); // pin, min, max, start
  master.servo.setServoInverse(1, true);
  master.servo.setServoRel(1, 0.01f);
  master.servo.registerServo(2, 5, 110, 100); // pin, min, max, start
  master.servo.setServoInverse(2, true);
  master.servo.setServoRel(2, 0.01f);
  master.servo.registerServo(3, 5, 110, 100); // pin, min, max, start
  master.servo.setServoInverse(3, true);
  master.servo.setServoRel(3, 0.01f);
#endif

#ifdef CASTAFIORE
  master.servo.registerServo(27, 18, 100, 50);
#endif

#ifdef CAMEMBERT
#ifdef ESP32
  //master.servo.registerServo(27, 0.5f, 0.7f, 1.0f); // pin, min, max, start
  master.servo.registerServo(27, 90, 160, 80); // pin, min, max, start
  //master.servo.setServoInverse(0, false);
#else
  // master.servo.registerServo(14, 140, 50, 139); // pin, min, max, start
#endif
#endif

#ifdef AMPOULE
  // COULEUR
  master.led.registerLedStrip(0, 21, 10, NEO_GRB + NEO_KHZ800); // index, pin, nbLeds, ledType
  // master.led.registerLedStrip(0, 21, 10, NEO_RGB + NEO_KHZ800); // index, pin, nbLeds, ledType
  // NON
  master.servo.registerServo(15, 0, 180, 90); // pin, min, max, start
  // master.servo.registerServo(23, 0.0f, 1.0f, 0.5f); // pin, min, max, start
#endif

#ifdef BASE
  // led
  master.led.registerLedStrip(0, 21, 10, NEO_GRB + NEO_KHZ800);
  // OUI
  master.servo.registerServo(27, 50, 140, 70); // pin, min, max, start
  master.servo.setServoInverse(0, false);
  // PIED
  master.servo.registerServo(12, 60, 120, 120); // pin, min, max, start
  master.servo.setServoInverse(1, true);
  //rotation
  master.motorwing.registerStepper(14, 15, 32, 13);
  master.motorwing.stepperSetMaxSpeed(0, 500.0f); // MAX VALUE WITH CHINESE DRIVER
  master.motorwing.stepperSetAccel(0, 250.0f);
  // TODO #define REVOLUTION_STEPS 1025 ?
#endif

#ifdef BOBINE
  master.led.registerLedStrip(0, 21, 300, NEO_GRB + NEO_KHZ800);
  master.led.setWifiDebug(0, false);
  master.led.setBrightness(0.4f);
  master.motorwing.registerShieldv2Stepper(200, MotorShield2Manager::StepperPort::M3_M4);
#endif

#ifdef BOBINETTE
 master.motorwing.registerStepper(16, 17);
#endif

#ifdef BOUCHE
  master.led.registerLedStrip(0, 21, 300, NEO_GRB + NEO_KHZ800);
  master.led.setWifiDebug(0, false);
  master.led.setBrightness(0.4f);
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
  // TODO: blip blip
#endif
}

void loop()
{
  master.update();  
}
