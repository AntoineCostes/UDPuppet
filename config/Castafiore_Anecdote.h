#pragma once

const String BOARD_NAME = "Castafiore";    // name used for mDNS services 
const BoardType BOARD_TYPE = HUZZAH32;
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 12003;

#define ALLOW_ESP32_SERVO_UNSAFE_PINS // 2, 4, 12, 13, 14, 15, 16, 17, 21, 22, 23, 25, 32

#define WEBSERVER
#define HAS_MUSICMAKER // uses pins (14, 15, 32, 33) on esp32, (0, 2, 15, 16) on esp8266 // and (11, 12, 13) for manipulating files on SD
#define FRAME_SIZE 4

#define NUM_STRIPS 1
const float LED_INTENSITY = 0.8f;
const UDPuppetLedStrip LED_STRIPS[NUM_STRIPS] = 
{
    // pin, num leds, GRB?, wifiDebug, useInSequences
    {21, 10, true, false, false}
};

#define NUM_SERVOS 1
const UDPuppetServo SERVOS[NUM_SERVOS] = 
{
    // pin, min [0,180], max [0,180], start [0,180], inverse, isMultiServo, useInSequences
    {22, 5, 80, 5, false, false, true}
};

#define NUM_BUTTONS 1
const UDPuppetButton BUTTONS[NUM_BUTTONS] = 
{
    // pin, longPressMs
    {17, 1200}
};