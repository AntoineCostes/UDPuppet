#pragma once

const String BOARD_NAME = "Roomba";    // name used for mDNS services 
const BoardType BOARD_TYPE = HUZZAH32;
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 10005;

#define FRAME_SIZE 1

#define NUM_STRIPS 1
const float LED_INTENSITY = 0.8f;
const UDPuppetLedStrip LED_STRIPS[NUM_STRIPS] = 
{
    // pin, num leds, GRB?, wifiDebug, useInSequences
    {12, 10, true, true, true}
};

#define HAS_ROOMBA
#define ROOMBA_IN_PIN 33
#define ROOMBA_OUT_PIN 32
#define ROOMBA_WAKE_PIN 15