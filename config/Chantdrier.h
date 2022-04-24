#pragma once

const String BOARD_NAME = "Chantdrier";    // name used for mDNS services 

// BOARD TYPE
const BoardType BOARD_TYPE = HUZZAH32;
#define BATTERY_PIN A13

// OSC PARAMETERS
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 12002;

// COMPONENTS
#define HAS_SERVO
#define HAS_MUSICMAKER
#define HAS_MULTISERVO

