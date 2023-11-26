#pragma once

const String BOARD_NAME = "Camembert_8266";    // name used for mDNS services 
const BoardType BOARD_TYPE = HUZZAH8266;
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 12001;

#define HAS_MUSICMAKER // uses pins (14, 15, 32, 33) on esp32, (0, 2, 15, 16) on esp8266 // and (11, 12, 13) for manipulating files on SD
#define WEBSERVER
// #define USE_SD

#define SEQUENCE_FRAME_SIZE 1

#define NUM_SERVOS 1
const UDPuppetServo SERVOS[NUM_SERVOS] = 
{
    // pin, min [0,180], max [0,180], start [0,180], inverse, isMultiServo, useInSequences
    {4, 0, 180, 50, false, false, true}
};
