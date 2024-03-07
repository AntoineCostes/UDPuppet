#pragma once

const String BOARD_NAME = "VacheQuiBlague";    // name used for mDNS services 
const BoardType BOARD_TYPE = XIAO_C3;
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 9000;

// #define HAS_SERIAL_MP3


#define SEQUENCE_FRAME_SIZE 1

#define NUM_SERVOS 1
const UDPuppetServo SERVOS[NUM_SERVOS] = 
{
    // pin, min [0,180], max [0,180], start [0,180], inverse, isMultiServo, useInSequences
    {10, 0, 80, 0, false, false, true}

};

#define NUM_STRIPS 3
const float LED_INTENSITY = 1.0f;
const UDPuppetLedStrip LED_STRIPS[NUM_STRIPS] = 
{
    // pin, num leds, GRB?, wifiDebug, useInSequences
    {5, 20, true, true, false}, // TODO remove
    {8, 20, true, false, false},
    {7, 20, false, true, false}
};


#define NUM_BUTTONS 1
const UDPuppetButton BUTTONS[NUM_BUTTONS] = 
{
    // pin, shortPressMs, longPressMs, clearOnPressed, playSequencesOnShort, cancelSoundOnLongPress, enableHotspotOnLong
    {9, 5, 1500, UDPuppetButtonBehavior{true, true, false, false}}
};