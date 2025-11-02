#pragma once

const String BOARD_NAME = "Lu7";    // name used for mDNS services 
const BoardType BOARD_TYPE = HUZZAH32;
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 12002;

// Recommended pins to attach Servo on a ESP32 are : 2, 4, 12-19, 21-23, 25-27, 32-33
// Recommended pins to attach Servo on a ESP32-S3 are : 1-21,35-45,47
// Recommended pins to attach Servo on a ESP32-C3 are : 1-7, 9-10, 18-21
#define ALLOW_ESP32_SERVO_UNRECOMMENDED_PINS

#define SEQUENCE_FRAME_SIZE 4

#define NUM_SERVOS 8
const UDPuppetServo SERVOS[NUM_SERVOS] = 
{
    // pin, min [0,180], max [0,180], start [0,180], inverse, isMultiServo, useInSequences
    {0, 0, 180, 90, true, true, true},
    {1, 0, 180, 90, true, true, true},
    {2, 0, 180, 90, true, true, true},
    {3, 0, 180, 90, true, true, true},
    {4, 0, 180, 90, true, true, true},
    {5, 0, 180, 90, true, true, true},
    {6, 0, 180, 90, true, true, true},
    {7, 0, 180, 90, true, true, true}
};