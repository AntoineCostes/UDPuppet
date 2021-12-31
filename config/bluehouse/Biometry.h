#pragma once

#define BOARD_BIOMETRY
const String BOARD_NAME = "Biometry";    // name used for mDNS services 
const BoardType BOARD_TYPE = HUZZAH32;

// OSC PARAMETERS
const int OSC_LISTENING_PORT = 9001;
const int OSC_TARGET_PORT = 12000;

// COMPONENTS
#define NUM_LEDS 2
const float LED_INTENSITY = 0.3f;

const UDPuppetLedStrip LED_STRIPS[NUM_LEDS] = 
{
    {"led one", 34, 10, true},
    {"led two", 39, 10, true}
};

#define NUM_HCSR04 2

const UDPuppetHCSR04 ULTRASONICS[NUM_HCSR04] = 
{
    {"left hand", 14, 32}//,
     {"right hand", 27, 12}
};

#define NUM_SERVOS 4

const UDPuppetServo SERVOS[NUM_SERVOS] = 
{
    // nice name, pin, min [0,1], max [0,1], start [0,1]
    {"servo 1", 21, 0.0f, 1.0f, 0.0f},
    {"servo 2", 17, 0.0f, 1.0f, 0.0f},
    {"servo 3", 22, 0.0f, 1.0f, 0.0f},
    {"servo 4", 23, 0.0f, 1.0f, 0.0f}
};