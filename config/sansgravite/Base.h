#pragma once

const String BOARD_NAME = "Base";    // name used for mDNS services 

// BOARD TYPE
const BoardType BOARD_TYPE = HUZZAH32;

// OSC PARAMETERS
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 10001;

// COMPONENTS
#define HAS_SD_WING
#define HAS_LED
#define HAS_SERVO
#define HAS_MOTORWING