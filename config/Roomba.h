#pragma once

#define BOARD_ROOMBA;
const String BOARD_NAME = "Roomba";    // name used for mDNS services 
const BoardType BOARD_TYPE = HUZZAH32;

// OSC PARAMETERS
const int OSC_LISTENING_PORT = 9000;
const int OSC_TARGET_PORT = 10005;

// COMPONENTS
#define HAS_LED
#define HAS_ROOMBA