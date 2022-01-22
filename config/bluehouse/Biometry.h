#pragma once

#define BOARD_BIOMETRY
const String BOARD_NAME = "Biometry";    // name used for mDNS services 
const BoardType BOARD_TYPE = HUZZAH32;

// OSC PARAMETERS
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 12000;

const bool LED_WIFI_DEBUG_ALL = false;

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
    {"right hand", 19, 18, true},
    {"left hand", 32, 15, true}
};



#define NUM_SERVOS 5

const UDPuppetServo SERVOS[NUM_SERVOS] = 
{
    // nice name, pin, min [0,1], max [0,1], start [0,1]
    
    {"servo 1", 22, 0.0f, .6f, 1.0},
    {"servo 2", 23, 0.0f, .6f, 0.0f},
    {"servo 3", 14, 0.0f, .6f, 1.0f},
    {"servo 4", 21, 0.0f, .6f, 0.0f},
    {"servo 5", 17, 0.0f, .6f, 1.0f}
};
