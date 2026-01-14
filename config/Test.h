#pragma once

const String BOARD_NAME = "Castafiore";    // name used for mDNS services 
const BoardType BOARD_TYPE = XIAO_C3;
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 12003;

// Recommended pins to attach Servo on a ESP32 are : 2, 4, 12-19, 21-23, 25-27, 32-33
// Recommended pins to attach Servo on a ESP32-S3 are : 1-21,35-45,47
// Recommended pins to attach Servo on a ESP32-C3 are : 1-7, 9-10, 18-21
#define ALLOW_ESP32_SERVO_UNRECOMMENDED_PINS

#define HAS_MUSICMAKER // uses pins (14, 15, 32, 33) on esp32, (0, 2, 15, 16) on esp8266 // and (11, 12, 13) for manipulating files on SD
// const int REPERTOIRE_LENGTH = 3;
// const String REPERTOIRE[REPERTOIRE_LENGTH] = {"nuit", "contine", "graves"};

#define HAS_ADALOGGER_WING // uses pins (5, 18 ,19)
// #define HAS_MOTORWING
#define HAS_STEPPER_DRIVER

#define SEQUENCE_FRAME_SIZE 1

// #define NUM_STRIPS 1
// const float LED_INTENSITY = 1.0f;
// const UDPuppetLedStrip LED_STRIPS[NUM_STRIPS] = 
// {
//     // pin, num leds, GRB, wifiDebug, useInSequences
//     {21, 10, true, true, false}
// };

#define NUM_SERVOS 1
const UDPuppetServo SERVOS[NUM_SERVOS] = 
{
    // pin, min [0,180], max [0,180], start [0,180], inverse, isMultiServo, useInSequences
    {27, 0, 180, 0, false, false, true}

};

#define NUM_BUTTONS 1
const UDPuppetButton BUTTONS[NUM_BUTTONS] = 
{
    // pin, shortPressMs, longPressMs, clearOnPressed, playSequencesOnShort, cancelSoundOnLongPress, enableHotspotOnLong
    {17, 5, 2000, UDPuppetButtonBehavior{true, true, true, false}}
};

#define NUM_HCSR04 1
const UDPuppetHCSR04 HCSR04[NUM_HCSR04] = 
{
    // pin, shortPressMs, longPressMs, clearOnPressed, playSequencesOnShort, cancelSoundOnLongPress, enableHotspotOnLong
    {"distance", 20, 21, false}
};