#pragma once

#define BOARD_BOBINE;
const String BOARD_NAME = "Bobine";    // name used for mDNS services 
const BoardType BOARD_TYPE = HUZZAH32;

// OSC PARAMETERS
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 10002;

// COMPONENTS
#define HAS_SD_WING
#define HAS_LED
#define HAS_MOTORWING
#define FRAME_SIZE 1 // 1 stepper