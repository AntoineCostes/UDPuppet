#pragma once

const String BOARD_NAME = "Castafiore";    // name used for mDNS services 
const BoardType BOARD_TYPE = HUZZAH32;
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 12003;

#define NUM_LEDS 1
const float LED_INTENSITY = 0.8f;
const UDPuppetLedStrip LED_STRIPS[NUM_LEDS] = 
{
    // pin, num leds, RGB/GRB, wifiDebug
    {21, 10, true, false}
};

#define NUM_SERVOS 1
const UDPuppetServo SERVOS[NUM_SERVOS] = 
{
    // pin, min [0,1], max [0,1], start [0,1]
    {22, 0.0f, 1.0f, 1.0f, false}
};