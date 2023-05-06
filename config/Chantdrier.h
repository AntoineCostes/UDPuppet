#pragma once

const String BOARD_NAME = "Chantdrier";    // name used for mDNS services 
const BoardType BOARD_TYPE = HUZZAH32;
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 12003;

#define BUTTON_JUKEBOX
#define HAS_MUSICMAKER

#define FRAME_SIZE 4 // 4 servos

#define NUM_SERVOS 4
const UDPuppetServo SERVOS[NUM_SERVOS] = 
{
    // pin, min [0,180], max [0,180], start [0,180], inverse, isMultiServo
    {0, 10, 110, 100, true, true},
    {1, 5, 110, 100, true, true},
    {2, 5, 110, 100, true, true},
    {3, 5, 110, 100, true, true}
};