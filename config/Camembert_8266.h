#pragma once

const String BOARD_NAME = "Camembert_8266";    // name used for mDNS services 
const BoardType BOARD_TYPE = HUZZAH8266;
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 12001;

#define HAS_MUSICMAKER
#define WEBSERVER
// #define USE_SD

#define FRAME_SIZE 1

#define NUM_SERVOS 1
const UDPuppetServo SERVOS[NUM_SERVOS] = 
{
    // pin, min [0,180], max [0,180], start [0,180], inverse, isMultiServo, useInSequences
    {4, 0, 180, 50, false, false, true}
};
