#pragma once
#include <Arduino.h>

#define AMPOULE

// BOARD PARAMETERS
const String BOARD_NAME = "Ampoule";    // name used for mDNS services 
enum BoardType
{
    HUZZAH32
};
const BoardType BOARD_TYPE = HUZZAH32;    // defines usable pins

// WIFI PARAMETERS
const long DELAY_BEFORE_RECONNECT_MS = 3000;
const long CONNECTION_TIMEOUT_MS = 10000;

// OSC PARAMETERS
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 10000;
const String OSC_TARGET_IP = "192.168.43.14";

// LED PARAMETERS
const long LED_REFRESH_MS = 20;

// SERVO PARAMETERS
const int SERVO_MAX = 180;
