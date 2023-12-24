#pragma once

const String BOARD_NAME = "XiaoLed";    // name used for mDNS services 
const BoardType BOARD_TYPE = XIAO_C3;
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 12003;

#define SEQUENCE_FRAME_SIZE 3

#define NUM_STRIPS 1

const float LED_INTENSITY = 1.0f;
const UDPuppetLedStrip LED_STRIPS[NUM_STRIPS] = 
{
    // pin, num leds, GRB, wifiDebug, useInSequences
    {10, 20, true, true, true}
};