#pragma once

const String BOARD_NAME = "Test";    // name used for mDNS services 
const BoardType BOARD_TYPE = HUZZAH32;
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 12003;

// #define NUM_LEDS 1
// const float LED_INTENSITY = 0.8f;
// const UDPuppetLedStrip LED_STRIPS[NUM_LEDS] = 
// {
//     // pin, num leds, RGB/GRB, wifiDebug
//     {21, 10, true, false}
// };

#define FRAME_SIZE 3

#define NUM_SERVOS 3
const UDPuppetServo SERVOS[NUM_SERVOS] = 
{
    // pin, min [0,180], max [0,180], start [0,180], inverse, isMultiServo
    {0, 0, 90, 0, false, true},
    {1, 0, 180, 0, false, true},
    {27, 0, 180, 90, false, false}

};