#pragma once

const String BOARD_NAME = "Camembert";    // name used for mDNS services 
const BoardType BOARD_TYPE = XIAO_C3;
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 12001;

#define ALLOW_ESP32_SERVO_UNRECOMMENDED_PINS // Recommended pins to attach Servo on a ESP32-C3 are : 1-7, 9-10, 18-21

#define SEQUENCE_FRAME_SIZE 1

#define NUM_SERVOS 1
const UDPuppetServo SERVOS[NUM_SERVOS] = 
{
    // pin, min [0,180], max [0,180], start [0,180], inverse, isMultiServo, useInSequences
    {10, 0, 100, 100, false, false, true}
};
