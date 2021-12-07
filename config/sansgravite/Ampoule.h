#pragma once

#define BOARD_AMPOULE
const String BOARD_NAME = "Ampoule";    // name used for mDNS services 
const BoardType BOARD_TYPE = HUZZAH32;

// OSC PARAMETERS
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 10000;

// COMPONENTS
#define HAS_LED
#define HAS_SERVO
#define HAS_SD_WING
#define FRAME_SIZE 4 // 1 pixel (RGB) + 1 servo