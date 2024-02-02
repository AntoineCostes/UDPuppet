#pragma once

const String BOARD_NAME = "Chantdrier";    // name used for mDNS services 
const BoardType BOARD_TYPE = HUZZAH32;
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 12002;

#define WEBSERVER
#define HAS_MUSICMAKER // uses pins (14, 15, 32, 33) on esp32, (0, 2, 15, 16) on esp8266 // and (11, 12, 13) for manipulating files on SD

#define SEQUENCE_FRAME_SIZE 4 // 4 servos

#define NUM_SERVOS 4
const UDPuppetServo SERVOS[NUM_SERVOS] = 
{
    // pin, min [0,180], max [0,180], start [0,180], inverse, isMultiServo, useInSequences
    {0, 10, 110, 15, true, true, true},
    {1, 10, 110, 15, true, true, true},
    {2, 10, 110, 15, true, true, true},
    {3, 10, 110, 15, true, true, true}
};
