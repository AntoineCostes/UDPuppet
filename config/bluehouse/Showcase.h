#pragma once

#define BOARD_SHOWCASE
const String BOARD_NAME = "Showcase";    // name used for mDNS services 
const BoardType BOARD_TYPE = HUZZAH32;

// OSC PARAMETERS
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 12000;

// COMPONENTS
#define HAS_LED
const float LED_INTENSITY = 0.8f;
const int NUM_LED_STRIPS = 5;
struct UDPuppetLedStrip
{
    String niceName;
    int pin;
    int numLeds;
    bool GRB;
};

const UDPuppetLedStrip LED_STRIPS[NUM_LED_STRIPS] = 
{
    {"led 1", 21, 10, false},
    {"led 2", 17, 10, false},
    {"led 3", 16, 10, false},
    {"led 4", 19, 10, false},
    {"led 5", 18, 10, false}
};

#define HAS_SERVO
const int NUM_SERVOS = 2;
struct UDPuppetServo
{
    String niceName;
    int pin;
    float startPosition;
};

const UDPuppetServo SERVOS[NUM_SERVOS] = 
{
    {"servo 1", 14, 0.5f},
    {"servo 2", 32, 0.5f},
};
   