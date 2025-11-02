#pragma once

const String BOARD_NAME = "XiaoBox";    // name used for mDNS services 
const BoardType BOARD_TYPE = XIAO_C3;
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 12003;

#define SEQUENCE_FRAME_SIZE 1

#define NUM_SERVOS 3
const UDPuppetServo SERVOS[NUM_SERVOS] = 
{
    // pin, min [0,180], max [0,180], start [0,180], inverse, isMultiServo, useInSequences
    {1, 0, 180, 90, false, false, true},
    {2, 0, 180, 90, false, false, true},
    {3, 0, 180, 90, false, false, true}

};
