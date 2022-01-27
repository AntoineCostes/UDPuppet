#pragma once

// BOARD PARAMETERS
const String BOARD_NAME = "Test";    // name used for mDNS services 
const BoardType BOARD_TYPE = HUZZAH32;

// NETWORK
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 12000;

// COMPONENTS
#define NUM_LEDS 1
const float LED_INTENSITY = 0.8f;
const UDPuppetLedStrip LED_STRIPS[NUM_LEDS] = 
{
    // nice name, pin, num leds, RGB/GRB
    {"led 1", 21, 10, true}
};

#define NUM_SERVOS 1
const UDPuppetServo SERVOS[NUM_SERVOS] = 
{
    // nice name, pin, min [0,1], max [0,1], start [0,1]
    {"servo", 22, 0.0f, 1.0f, 1.0f}
};