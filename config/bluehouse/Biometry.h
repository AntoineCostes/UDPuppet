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
    {"led one", 27, 10, true},
    {"led two", 21, 10, true}
};

#define NUM_HCSR04 1

const UDPuppetHCSR04 ULTRASONICS[NUM_HCSR04] = 
{
    {"left hand", 14, 32}//,
    // {"right hand", 14, 15}
};