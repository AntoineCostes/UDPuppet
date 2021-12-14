#pragma once

#define BOARD_BIOMETRY
const String BOARD_NAME = "Biometry";    // name used for mDNS services 
const BoardType BOARD_TYPE = HUZZAH32;

// OSC PARAMETERS
const int OSC_LISTENING_PORT = 9001;
const int OSC_TARGET_PORT = 12000;

// COMPONENTS
#define HAS_LED
const float LED_INTENSITY = 0.3f;
const int NUM_LED_STRIPS = 2;
struct NeoPixelStrip
{
    String niceName;
    int pin;
    int numLeds;
    bool GRB;
};

const NeoPixelStrip LED_STRIPS[NUM_LED_STRIPS] = 
{
    {"led one", 21, 10, false},
    {"led two", 27, 10, false}
};

#define HAS_HCSR04
const int NUM_HCSR04 = 1;
struct HcSr04
{
    String niceName;
    int triggerPin;
    int echoPin;
};

const HcSr04 ULTRASONICS[NUM_HCSR04] = 
{
    {"left hand", 32, 14}//,
    // {"right hand", 14, 15}
};