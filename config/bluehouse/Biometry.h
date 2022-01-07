#pragma once

#define BOARD_BIOMETRY
const String BOARD_NAME = "Biometry";    // name used for mDNS services 
const BoardType BOARD_TYPE = HUZZAH32;

// OSC PARAMETERS
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 12000;

// COMPONENTS

#define NUM_LEDS 3
const float LED_INTENSITY = 0.3f;

const UDPuppetLedStrip LED_STRIPS[NUM_LEDS] = 
{
    {"led one", 4, 10, true},
    {"led two", 27, 10, true},
    {"led two", 12, 10, true}
};

#define NUM_HCSR04 2

const UDPuppetHCSR04 ULTRASONICS[NUM_HCSR04] = 
{
    {"left hand", 19, 18},
    {"right hand", 32, 15}
};


#define NUM_SERVOS 5

const UDPuppetServo SERVOS[NUM_SERVOS] = 
{
    // nice name, pin, min [0,1], max [0,1], start [0,1]
    {"servo 1", 23, 0.0f, 1.0f, 0.0f},
    {"servo 2", 22, 0.0f, 1.0f, 0.0f},
    {"servo 3", 14, 0.0f, 1.0f, 0.0f},
    {"servo 4", 21, 0.0f, 1.0f, 0.0f},
    {"servo 4", 17, 0.0f, 1.0f, 0.0f}
};