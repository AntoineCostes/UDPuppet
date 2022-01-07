#pragma once

#define BOARD_BIOMETRY
const String BOARD_NAME = "Motors";    // name used for mDNS services 
const BoardType BOARD_TYPE = HUZZAH32;

// OSC PARAMETERS
const int OSC_LISTENING_PORT = 9001;
const int OSC_TARGET_PORT = 12000;

// COMPONENTS
#define NUM_SERVOS 4

const UDPuppetServo SERVOS[NUM_SERVOS] = 
{
    // nice name, pin, min [0,1], max [0,1], start [0,1]
    {"servo 1", 23, 0.0f, 1.0f, 0.3f},
    {"servo 2", 22, 0.0f, 1.0f, 0.0f},
    {"servo 3", 14, 0.0f, 1.0f, 0.0f},
    {"servo 4", 32, 0.0f, 1.0f, 0.0f}
};