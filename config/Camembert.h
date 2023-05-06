#pragma once

const String BOARD_NAME = "Camembert";    // name used for mDNS services 
const BoardType BOARD_TYPE = HUZZAH32;
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 12001;

#define HAS_MUSICMAKER

#define FRAME_SIZE 1

#define NUM_SERVOS 1
const UDPuppetServo SERVOS[NUM_SERVOS] = 
{
    // pin, min [0,180], max [0,180], start [0,180], inverse, isMultiServo
    {27, 110, 150, 110, true, false, true}
   // {27, 90, 160, 150, true, false}
};
