#pragma once

const String BOARD_NAME = "Castafiore";    // name used for mDNS services 
const BoardType BOARD_TYPE = HUZZAH32;
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 12003;

#define SERVO_ALLOW_UNSAFE_PINS // pins 2, 4, 12, 13, 14, 15, 16, 17, 21, 22, 23, 25, 32

//#define HAS_MUSICMAKER // uses pins 14, 15, 32, 33
#define FRAME_SIZE 1

#define NUM_SERVOS 1
const UDPuppetServo SERVOS[NUM_SERVOS] = 
{
    // pin, min [0,180], max [0,180], start [0,180], inverse, isMultiServo, useInSequences
    {27, 0, 180, 0, false, false, true}

};