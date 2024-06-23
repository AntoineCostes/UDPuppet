#pragma once

const String BOARD_NAME = "Castafiore";    // name used for mDNS services 
const BoardType BOARD_TYPE = XIAO_C3;
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 12003;

#define ALLOW_ESP32_SERVO_UNRECOMMENDED_PINS 

#define SEQUENCE_FRAME_SIZE 5

#define NUM_SERVOS 2
const UDPuppetServo SERVOS[NUM_SERVOS] = 
{
    // pin, min [0,180], max [0,180], start [0,180], inverse, isMultiServo, useInSequences
    {10, 50, 140, 0, false, false, true}, // min DS06 = 40
    // {6, 0, 180, 0, false, false, true},
    {9, 0, 180, 0, false, false, true}

};

#define NUM_STRIPS 1
const float LED_INTENSITY = 1.0f;
const UDPuppetLedStrip LED_STRIPS[NUM_STRIPS] = 
{
    // pin, num leds, GRB, wifiDebug, useInSequences
    {6, 20, true, true, true}
};

#define NUM_BUTTONS 1
const UDPuppetButton BUTTONS[NUM_BUTTONS] = 
{
    // pin, longPressMs
    {8, 2000}
};