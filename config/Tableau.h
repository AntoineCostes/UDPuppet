#pragma once

const String BOARD_NAME = "Tableau";    // name used for mDNS services 
const BoardType BOARD_TYPE = XIAO_S3;
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 12002;

// #define HAS_SERIAL_MP3 // uses pins 6 and 7
// #define COIN_PIN 8

#define SEQUENCE_FRAME_SIZE 8 // 8 servos

#define NUM_STRIPS 1
const float LED_INTENSITY = 1.0f;
const UDPuppetLedStrip LED_STRIPS[NUM_STRIPS] = 
{
    // pin, num leds, GRB, wifiDebug, useInSequences
    {10, 20, true, true, false}
};

#define NUM_SERVOS 8
const UDPuppetServo SERVOS[NUM_SERVOS] = 
{
    // pin, min [0,180], max [0,180], start [0,180], inverse, isMultiServo, useInSequences
    {0, 0, 180, 90, false, true, true},
    {1, 0, 180, 90, false, true, true},
    {2, 0, 180, 90, false, true, true},
    {3, 0, 180, 90, false, true, true},
    {4, 0, 180, 90, false, true, true},
    {5, 0, 180, 90, false, true, true},
    {6, 0, 180, 90, false, true, true},
    {7, 0, 180, 90, false, true, true}
};
