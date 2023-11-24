#include "Config.h"
#include "src/common/PuppetMaster.h"

// TODO
// OSC error feedback
// used pins feedback => give names to reservedPins ?
// make battery monitoring a pref option (ESP32)
// change debug from Chataigne
// checkComponents
// readFlashParameters for 8266
// implement microSeconds for servo
// port in handshake ?
// JSON config files ?
// component bool checkRange(float/int min, float/int max)

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

  #ifdef NUM_STRIPS
  for (int i = 0; i < NUM_STRIPS; i++)
    master.led.registerLedStrip(LED_STRIPS[i].pin, LED_STRIPS[i].numLeds, LED_STRIPS[i].GRB?NEO_GRB:NEO_RGB + NEO_KHZ800, LED_STRIPS[i].wifiDebug, LED_STRIPS[i].useInSequences);
    master.led.setBrightness(LED_INTENSITY);
    master.led.clear();
  #endif

  #ifdef NUM_SERVOS
  for (int i = 0; i < NUM_SERVOS; i++)
    master.servo.registerServo(SERVOS[i].pin, SERVOS[i].min, SERVOS[i].max, SERVOS[i].start, SERVOS[i].inverse, SERVOS[i].isMultiServo, SERVOS[i].useInSequences);
  #endif

  #ifdef NUM_BUTTONS
  for (int i = 0; i < NUM_BUTTONS; i++)
    master.button.registerButton(BUTTONS[i].pin, BUTTONS[i].longPressMs);
  #endif

  #ifdef NUM_HCSR04
  for (int i = 0; i < NUM_HCSR04; i++)
    master.sensorMgr.registerHCSR04Reader(ULTRASONICS[i].niceName, ULTRASONICS[i].triggerPin, ULTRASONICS[i].echoPin, ULTRASONICS[i].active);
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

}

void loop()
{
  master.update();  
}
