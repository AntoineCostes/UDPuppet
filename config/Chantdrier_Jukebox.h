#pragma once

const String BOARD_NAME = "Chantdrier";    // name used for mDNS services 
const BoardType BOARD_TYPE = HUZZAH32;
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 12002;

#define WEBSERVER
#define HAS_MUSICMAKER
#define BUTTON_JUKEBOX
const int REPERTOIRE_LENGTH = 2;
const String REPERTOIRE[REPERTOIRE_LENGTH] = {"nuit", "contine"};

#define FRAME_SIZE 4 // 4 servos

#define NUM_STRIPS 1
const float LED_INTENSITY = 1.0f;
const UDPuppetLedStrip LED_STRIPS[NUM_STRIPS] = 
{
    // pin, num leds, GRB, wifiDebug, useInSequences
    {21, 10, true, true, false}
};

#define NUM_SERVOS 4
const UDPuppetServo SERVOS[NUM_SERVOS] = 
{
    // pin, min [0,180], max [0,180], start [0,180], inverse, isMultiServo, useInSequences
    {0, 10, 110, 15, true, true, true},
    {1, 10, 110, 15, true, true, true},
    {2, 10, 110, 15, true, true, true},
    {3, 10, 110, 15, true, true, true}
};

#define NUM_BUTTONS 1
const UDPuppetButton BUTTONS[NUM_BUTTONS] = 
{
    // pin, longPressMs
    {17, 2000}
};