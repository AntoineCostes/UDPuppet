#pragma once

// BOARD PARAMETERS
const String BOARD_NAME = "Showcase";    // name used for mDNS services 
const BoardType BOARD_TYPE = HUZZAH32;

// OSC PARAMETERS
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 12000;

const bool LED_WIFI_DEBUG_ALL = true;

// COMPONENTS
#define NUM_LEDS 10
const float LED_INTENSITY = 0.8f;

const UDPuppetLedStrip LED_STRIPS[NUM_LEDS] = 
{
    // nice name, pin, num leds, RGB/GRB
    {"led 1", 21, 10, true},
    {"led 2", 17, 10, true},
    {"led 3", 16, 10, true},
    {"led 4", 19, 10, true},
    {"led 5", 18, 10, true},
    {"led 6", 23, 10, true},
    {"led 7", 22, 10, true},
    {"led 8", 14, 10, true},
    {"led 9", 32, 10, true},
    {"led 10", 15, 10, true}
};

#define NUM_HCSR04 1

const UDPuppetHCSR04 ULTRASONICS[NUM_HCSR04] = 
{
    {"left hand", 27, 12, false}
};
