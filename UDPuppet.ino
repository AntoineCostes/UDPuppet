#include "Config.h"
#include "src/common/PuppetMaster.h"
// dependencies:
// Adafruit_NeoPixel.h
// OSCMessage.h
// ESP32Servo.h
// Adafruit_MotorShield.h (v2)
// AccelStepper.h (modified)
// SPI.h
// SD.h
// FS.h
// WebServer.h

// TODO remove index
// TODO local libs
// TODO filemanager server
// TODO OTA
// TODO send both on 12000 and specific port ?
// TODO inputPins & outPutPins PinManager
// analog refresh => merge battery as AnalogReader child ?

// RAPPEL après avoir uploadé il faut rebooter la carte manuellement


PuppetMaster master;
long lastLoopTime = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  
  //while (!Serial) { delay(1); } // wait for serial console
  delay(2000); 

  master.initManager(); // TODO make singleton and rename ?

#ifdef TEST
  master.led.setBrightness(0.3f);
  //                            index, pin, nbLeds, ledType
  master.led.registerLedStrip(0, 21, 10, NEO_GRB + NEO_KHZ800);
#endif

#ifdef AMPOULE
  // couleur
  master.led.registerLedStrip(0, 21, 10, NEO_GRB + NEO_KHZ800); // index, pin, nbLeds, ledType
  // rotation tête
   master.servo.registerServo(0, 15, 0.5f); // index, pin, départ au centre
#endif

#ifdef BASE
  // led
  master.led.registerLedStrip(0, 21, 10, NEO_GRB + NEO_KHZ800);
  // pied
  // master.servo.registerServo(0, 27, 50, 140, 70); // index, pin, min, max, start
  master.servo.registerServo(0, 27, 1087, 2064, 1304);  // index, pin, pulseMin, pulseMax, pulseStart
  // cou
  // master.servo.registerServo(1, 12, 60, 120, 120); // index, pin, min, max, start
  master.servo.registerServo(1, 12, .3f, .6f, .6f); // index, pin, min, max, start
  //rotation
  master.motorwing.registerStepper(0, 14, 15, 32, 13);
  master.motorwing.stepperSetMaxSpeed(0, 500.0f); // MAX VALUE WITH CHINESE DRIVER
  master.motorwing.stepperSetAccel(0, 250.0f);
  // TODO #define REVOLUTION_STEPS 1025 ?
#endif

#ifdef BOBINE
  master.led.registerLedStrip(0, 23, 10, NEO_GRB + NEO_KHZ800);
  master.motorwing.registerShieldv2Stepper(0, 200, MotorShield2Manager::StepperPort::M3_M4);
#endif

#ifdef CORBEILLE
  master.led.registerLedStrip(0, 21, 10, NEO_GRB + NEO_KHZ800);
  master.led.setBrightness(0.2f);
  master.motorwing.addDCMotor(MotorShield2Manager::DCPort::M1);
  master.motorwing.addDCMotor(MotorShield2Manager::DCPort::M2);
#endif

#ifdef ROOMBA
  master.led.setBrightness(0.1f);
  master.led.registerLedStrip(0, 12, 10, NEO_GRB + NEO_KHZ800);
  master.roomba.registerRoomba(33, 32, 15);
  //master.roomba.setText(0, "Hello la cie !");
#endif

#ifdef MULTILED
  master.led.setBrightness(0.1f);
  master.led.registerLedStrip(0, 21, 12, NEO_RGB + NEO_KHZ800);
  master.led.registerLedStrip(1, 17, 12, NEO_RGB + NEO_KHZ800);
  master.led.registerLedStrip(2, 16, 12, NEO_RGB + NEO_KHZ800);
  master.led.registerLedStrip(3, 19, 12, NEO_RGB + NEO_KHZ800);
  master.led.registerLedStrip(4, 18, 12, NEO_RGB + NEO_KHZ800);
  master.led.registerLedStrip(5, 5, 12, NEO_RGB + NEO_KHZ800);
  master.led.registerLedStrip(6, 4, 12, NEO_RGB + NEO_KHZ800);
  
  master.led.registerLedStrip(7, 22, 12, NEO_RGB + NEO_KHZ800);
  master.led.registerLedStrip(8, 23, 12, NEO_RGB + NEO_KHZ800);
  master.led.registerLedStrip(9, 14, 12, NEO_RGB + NEO_KHZ800);
  master.led.registerLedStrip(10, 32, 12, NEO_RGB + NEO_KHZ800);
#endif

#ifdef BLUEHOUSE
  master.led.setBrightness(0.3f);
  master.led.registerLedStrip(0, 27, 12, NEO_RGB + NEO_KHZ800);
  //master.led.registerLedStrip(1, 27, 10, NEO_GRB + NEO_KHZ800);
#endif


#ifdef SERVOTEST
  // master.servo.registerServo(0, 21, 0, 180, 90); // index, pin, min, max, start
  // master.servo.registerServo(1, 17, 0, 180, 90);
  // master.servo.registerServo(2, 16, 0, 180, 90);
#endif
}

void loop()
{
  master.update();

  // ensure at least 1ms between frames
  if (millis() == lastLoopTime)
  {
    delay(1);
    lastLoopTime = millis();
  }
  
}
