#pragma once

const String BOARD_NAME = "Chantdrier_8266";    // name used for mDNS services 
const BoardType BOARD_TYPE = HUZZAH8266;
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 12002;

#define HAS_MUSICMAKER
#define BUTTON_JUKEBOX
const int REPERTOIRE_LENGTH = 2;
const String REPERTOIRE[REPERTOIRE_LENGTH] = {"nuit", "contine"};

#define FRAME_SIZE 4 // 4 servos

#define NUM_SERVOS 4
const UDPuppetServo SERVOS[NUM_SERVOS] = 
{
    // pin, min [0,180], max [0,180], start [0,180], inverse, isMultiServo, useInSequences
    {0, 10, 110, 15, true, true, true},
    {1, 10, 110, 15, true, true, true},
    {2, 10, 110, 15, true, true, true},
    {3, 10, 110, 15, true, true, true}
};
