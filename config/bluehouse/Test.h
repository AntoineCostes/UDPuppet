#pragma once

// BOARD PARAMETERS
const String BOARD_NAME = "Test";    // name used for mDNS services 
const BoardType BOARD_TYPE = THINGESP8266;

// OSC PARAMETERS
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 12000;

// COMPONENTS
#define NUM_SERVOS 1
// TODO: comment faire différents constructeurs ?
const UDPuppetServo SERVOS[NUM_SERVOS] = 
{
    // nice name, pin, min [0,1], max [0,1], start [0,1]
    {"servo 1", 23, 0.0f, 1.0f, 0.5f}
};
   