#include "Config.h"
#include "src/common/PuppetMaster.h"

// TODO
// no_wifi in credentials ?
// define behaviors ? or callback events in ino ?
// OSCQuery
// checkComponents
// remember target ip
// port in handshake ?

// remove initComponent
// checkInit only in Manager ?
// initComponent (when registering) and updating components automatically

// ERROR mode if prop was not registered successfully
// make childClass DebugLedStrip with communication methods

// RAPPEL après avoir uploadé il faut rebooter la carte manuellement

PuppetMaster master;

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(SETUP_DELAY_MS);

  master.initManager(); // TODO make singleton and rename ?

  #ifdef NUM_LEDS
  master.led.setBrightness(LED_INTENSITY);
  for (int i = 0; i < NUM_LEDS; i++)
    master.led.registerLedStrip(LED_STRIPS[i].pin, LED_STRIPS[i].numLeds, LED_STRIPS[i].GRB?NEO_GRB:NEO_RGB + NEO_KHZ800, LED_STRIPS[i].wifiDebug);
  #endif

  #ifdef NUM_SERVOS
  for (int i = 0; i < NUM_SERVOS; i++)
    master.servo.registerServo(SERVOS[i].pin, SERVOS[i].min, SERVOS[i].max, SERVOS[i].start, SERVOS[i].inverse, SERVOS[i].isMultiServo);
  #endif

  #ifdef NUM_BUTTONS
  for (int i = 0; i < NUM_BUTTONS; i++)
    master.button.registerButton(BUTTONS[i].pin, BUTTONS[i].longPressMs);
  #endif

  #ifdef NUM_HCSR04
  for (int i = 0; i < NUM_HCSR04; i++)
    master.sensorMgr.registerHCSR04Reader(ULTRASONICS[i].niceName, ULTRASONICS[i].triggerPin, ULTRASONICS[i].echoPin, ULTRASONICS[i].active);
  #endif
  
/*
#ifdef CHANTDRIER
  master.servo.registerServo(0, 10, 110, 100); // pin, min, max, start
  master.servo.setServoInverse(0, true);
  master.servo.servoGoTo(0, 0.01f);
  master.servo.registerServo(1, 5, 110, 100); // pin, min, max, start
  master.servo.setServoInverse(1, true);
  master.servo.servoGoTo(1, 0.01f);
  master.servo.registerServo(2, 5, 110, 100); // pin, min, max, start
  master.servo.setServoInverse(2, true);
  master.servo.servoGoTo(2, 0.01f);
  master.servo.registerServo(3, 5, 110, 100); // pin, min, max, start
  master.servo.setServoInverse(3, true);
  master.servo.servoGoTo(3, 0.01f);
#endif

#ifdef CASTAFIORE
  master.led.registerLedStrip(21, 10, NEO_GRB + NEO_KHZ800); 
  master.led.setWifiDebug(0, false);
  master.servo.registerServo(27, 5, 80, 5);
  master.button.registerButton(17, 1200);
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
  master.led.registerLedStrip(21, 10, NEO_GRB + NEO_KHZ800); // index, pin, nbLeds, ledType
  // master.led.registerLedStrip(21, 10, NEO_RGB + NEO_KHZ800); // index, pin, nbLeds, ledType
  // NON
  master.servo.registerServo(15, 0, 180, 90); // pin, min, max, start
  // master.servo.registerServo(23, 0.0f, 1.0f, 0.5f); // pin, min, max, start
#endif

#ifdef BASE
  // led
  master.led.registerLedStrip(21, 10, NEO_GRB + NEO_KHZ800);
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
  master.led.registerLedStrip(21, 300, NEO_GRB + NEO_KHZ800);
  master.led.setWifiDebug(0, false);
  master.led.setBrightness(0.4f);
  master.motorwing.registerShieldv2Stepper(200, MotorShield2Manager::StepperPort::M3_M4);
#endif

#ifdef BOBINETTE
 master.stepperdriver.registerStepper(16, 17, 19);
#endif

#ifdef BOUCHE
  master.led.registerLedStrip(21, 300, NEO_GRB + NEO_KHZ800);
  master.led.setWifiDebug(0, false);
  master.led.setBrightness(0.4f);
#endif

#ifdef CORBEILLE
  master.led.registerLedStrip(21, 10, NEO_GRB + NEO_KHZ800);
  master.led.setBrightness(0.2f);
  master.motorwing.addDCMotor(MotorShield2Manager::DCPort::M1);
  master.motorwing.addDCMotor(MotorShield2Manager::DCPort::M2);
#endif

#ifdef ROOMBA
  master.led.setBrightness(0.3f);
  master.led.registerLedStrip(12, 10, NEO_GRB + NEO_KHZ800);
  master.roomba.registerRoomba(33, 32, 15);
  master.roomba.setText(0, "Hello la cie !");
  // TODO: blip blip
#endif
*/
}

void loop()
{
  master.update();  
}
