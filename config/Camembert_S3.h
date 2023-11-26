#pragma once

const String BOARD_NAME = "Camembert";    // name used for mDNS services 
const BoardType BOARD_TYPE = HUZZAH32_S3;
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 12001;

#define HAS_MUSICMAKER
#define ALLOW_ESP32_SERVO_UNRECOMMENDED_PINS

#define SEQUENCE_FRAME_SIZE 1

#define NUM_STRIPS 1
const float LED_INTENSITY = 0.1f;
const UDPuppetLedStrip LED_STRIPS[NUM_STRIPS] = 
{
    // pin, num leds, GRB, wifiDebug, useInSequences
    {PIN_NEOPIXEL, 5, true, true, false}
};

#define NUM_SERVOS 1
const UDPuppetServo SERVOS[NUM_SERVOS] = 
{
    // pin, min [0,180], max [0,180], start [0,180], inverse, isMultiServo, useInSequences
    {10, 110, 150, 110, true, false, true}
};
