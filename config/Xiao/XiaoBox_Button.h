#pragma once

const String BOARD_NAME = "XiaoBox";    // name used for mDNS services 
const BoardType BOARD_TYPE = XIAO_C3;
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 12003;

#define HAS_SERIAL_MP3
#define BUTTON_JUKEBOX

#define SEQUENCE_FRAME_SIZE 1

#define NUM_SERVOS 1
const UDPuppetServo SERVOS[NUM_SERVOS] = 
{
    // pin, min [0,180], max [0,180], start [0,180], inverse, isMultiServo, useInSequences
    {10, 0, 180, 0, false, false, true}

};

#define NUM_BUTTONS 1
const UDPuppetButton BUTTONS[NUM_BUTTONS] = 
{
    // pin, shortPressMs, longPressMs, clearOnPressed, playSequencesOnShort, cancelSoundOnLongPress, enableHotspotOnLong
    {9, 5, 2000, UDPuppetButtonBehavior{true, true, true, false}}
};