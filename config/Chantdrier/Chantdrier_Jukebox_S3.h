#pragma once

const String BOARD_NAME = "Chantdrier";    // name used for mDNS services 
const BoardType BOARD_TYPE = HUZZAH32_S3;
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 12002;

#define USE_SD

#define HAS_MUSICMAKER // uses pins (14, 15, 32, 33) on esp32, (0, 2, 15, 16) on esp8266 // and (11, 12, 13) for manipulating files on SD
#define ALLOW_ESP32_SERVO_UNRECOMMENDED_PINS

#define COIN_PIN 16
#define JUKEBOX
const int REPERTOIRE_LENGTH = 2;
const String REPERTOIRE[REPERTOIRE_LENGTH] = {"nuit", "graves"};

#define SEQUENCE_FRAME_SIZE 4 // 4 servos + RGB

#define NUM_STRIPS 1
const float LED_INTENSITY = 1.0f;
const UDPuppetLedStrip LED_STRIPS[NUM_STRIPS] = 
{
    // pin, num leds, GRB, wifiDebug, useInSequences
    {18, 20, true, true, false}
};

#define NUM_SERVOS 4
const UDPuppetServo SERVOS[NUM_SERVOS] = 
{
    // pin, min [0,180], max [0,180], start [0,180], inverse, isMultiServo, useInSequences
    {0, 10, 95, 10, true, true, true},
    {1, 10, 95, 10, true, true, true},
    {2, 10, 95, 10, true, true, true},
    {3, 10, 95, 10, true, true, true}
};

#define NUM_BUTTONS 1
const UDPuppetButton BUTTONS[NUM_BUTTONS] = 
{
    // pin, shortPressMs, longPressMs, clearOnPressed, playSequencesOnShort, cancelSoundOnLongPress, enableHotspotOnLong
    {17, 5, 2000, UDPuppetButtonBehavior{true, true, true, false}}
};