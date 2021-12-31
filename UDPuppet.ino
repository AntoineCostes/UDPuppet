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

// RAPPEL après avoir uploadé il faut rebooter la carte manuellement

PuppetMaster master;
long lastLoopTime = 0;

// TODO FIX SERVO

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  
  //while (!Serial) { delay(1); } // wait for serial console
  delay(2000); 

  master.initManager(); // TODO make singleton and rename ?

  #ifdef NUM_LEDS
  master.ledMgr.setBrightness(LED_INTENSITY);
  for (int i = 0; i < NUM_LEDS; i++)
    master.ledMgr.registerLedStrip(i, LED_STRIPS[i].pin, LED_STRIPS[i].numLeds, LED_STRIPS[i].GRB?NEO_GRB:NEO_RGB + NEO_KHZ800);
  
  #endif

  #ifdef NUM_SERVOS
  for (int i = 0; i < NUM_SERVOS; i++)
    master.servoMgr.registerServo(i, SERVOS[i].pin, SERVOS[i].minPosition, SERVOS[i].maxPosition, SERVOS[i].startPosition);
  #endif

  #ifdef NUM_HCSR04
  for (int i = 0; i < NUM_HCSR04; i++)
    master.sensorMgr.registerHCSR04Reader(ULTRASONICS[i].niceName, ULTRASONICS[i].triggerPin, ULTRASONICS[i].echoPin);
  #endif
  
  digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
  master.update();
}
