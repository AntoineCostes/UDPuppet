#pragma once

#define AMPOULE

const String BOARD_NAME = "Ampoule";    // name used for mDNS services 

// BOARD TYPE
const BoardType BOARD_TYPE = HUZZAH32;

// OSC PARAMETERS
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 10000;

// COMPONENTS
//#define HAS_SD
#define HAS_LED
#define HAS_SERVO