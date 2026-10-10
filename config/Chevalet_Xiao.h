#pragma once

const String BOARD_NAME = "Chevalet";    // name used for mDNS services 
const BoardType BOARD_TYPE = XIAO_S3;
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 12000;

// #define HAS_LIPO

#define SEQUENCE_FRAME_SIZE 8 // 8 servos

#define NUM_STRIPS 1
const float LED_INTENSITY = 1.0f;
const UDPuppetLedStrip LED_STRIPS[NUM_STRIPS] = 
{
    // pin, num leds, GRB, wifiDebug, useInSequences
    {9, 20, true, false, false}
};

#define NUM_SERVOS 8
const UDPuppetServo SERVOS[NUM_SERVOS] = 
{
    // pin, min [0,180], max [0,180], start [0,180], inverse, isMultiServo, useInSequences
    {0, 10, 180, 0, false, true, true},
    {1, 0, 170, 0, false, true, true},
    {2, 0, 170, 0, false, true, true},
    {3, 0, 170, 0, false, true, true},
    {4, 10, 180, 0, false, true, true},
    {5, 0, 170, 0, false, true, true},
    {6, 10, 170, 0, false, true, true},
    {7, 10, 170, 0, false, true, true}
};

#define NUM_BUTTONS 2
const UDPuppetButton BUTTONS[NUM_BUTTONS] = 
{
    // pin, shortPressMs, longPressMs, clearOnPressed, playSequencesOnShort, cancelSoundOnLongPress, enableHotspotOnLong
    {7, 5, 3000, UDPuppetButtonBehavior{false, true, true, false}},
    {8, 5, 3000, UDPuppetButtonBehavior{true, false, false, true}}
};