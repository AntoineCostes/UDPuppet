#include "Config.h"
#include "src/common/PuppetMaster.h"

int batteryLevel;
float voltage;
#include <Arduino.h>
PuppetMaster master;
// #include "BLE.h"
// MyBLEServer ble("Camembert");

void setup() {
  Serial.begin(115200);

#ifdef LED_BUILTIN
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
#endif

  delay(SETUP_DELAY_MS);

  master.initManager();  // TODO make singleton and rename ?

#ifdef NUM_STRIPS
  for (int i = 0; i < NUM_STRIPS; i++)
    master.led.registerLedStrip(LED_STRIPS[i].pin, LED_STRIPS[i].numLeds, LED_STRIPS[i].GRB ? NEO_GRB : NEO_RGB + NEO_KHZ800, LED_STRIPS[i].wifiDebug, LED_STRIPS[i].useInSequences);
  master.led.setBrightness(LED_INTENSITY);
  master.led.clear();
#endif

#ifdef NUM_SERVOS
  for (int i = 0; i < NUM_SERVOS; i++)
    master.servo.registerServo(SERVOS[i].pin, SERVOS[i].min, SERVOS[i].max, SERVOS[i].start, SERVOS[i].inverse, SERVOS[i].isMultiServo, SERVOS[i].useInSequences);
#endif

#ifdef NUM_BUTTONS
  for (int i = 0; i < NUM_BUTTONS; i++)
    master.button.registerButton(BUTTONS[i].pin, BUTTONS[i].shortPressMs, BUTTONS[i].longPressMs, BUTTONS[i].behavior);
#endif

#ifdef NUM_HCSR04
  for (int i = 0; i < NUM_HCSR04; i++)
    master.sensorMgr.registerHCSR04(HCSR04[i].niceName, HCSR04[i].triggerPin, HCSR04[i].echoPin, HCSR04[i].active);
#endif

#ifdef HAS_ROOMBA
#ifdef ROOMBA_IN_PIN
#ifdef ROOMBA_OUT_PIN
#ifdef ROOMBA_WAKE_PIN
  master.roomba.registerRoomba(ROOMBA_IN_PIN, ROOMBA_OUT_PIN, ROOMBA_WAKE_PIN);
#endif
#endif
#endif
#endif

  // master.analog.registerAnalogReader("potar", A0);
  // TODO ADD STEPPERS AND MOTORWING

#ifdef NUM_STRIPS
  master.led.notify(LedStrip::Notification::READY);
#endif
  // Serial.println("BLE GO");
  // ble.init(); 
}

void loop() {
  master.update();
  // ble.update();

  // if (master.battery.level != batteryLevel)
  // {
  //   Serial.println("battery");
  //   Serial.println(master.battery.level);
  //   uint8_t level = (uint8_t)master.battery.level;
  //   ble.notifyByte(&level);
  //   batteryLevel = master.battery.level;
  // }
  
  // if (master.battery.voltage != voltage)
  // {
  //   Serial.println("battery");
  //   // ble.notifyFloat(master.battery.voltage);
  //   uint8_t level = (uint8_t)master.battery.level;
  //   ble.notifyByte(&level);
  //   voltage = master.battery.voltage;
  // }
}
