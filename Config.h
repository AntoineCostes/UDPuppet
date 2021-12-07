#pragma once
#include <Arduino.h>

enum BoardType
{
    HUZZAH32,
    HUZZAH8266 // not supported yet
};

///////////////////////////////
// NETWORKS - DEFAULTS TO LENET
///////////////////////////////
//#define SANS_GRAVITE
//#define COSTES
//#define SUNNY

////////////////////////////////////
// HARDWARE CONFIG - CHOOSE ONE FILE
////////////////////////////////////
// #include "config/sansgravite/Ampoule.h"
// #include "config/sansgravite/Base.h"
// #include "config/sansgravite/Bobine.h"
// #include "config/sansgravite/Corbeille.h"
// #include "config/Roomba.h"
#include "config/bluehouse/Biometry.h"


// WIFI PARAMETERS
#if defined(SANS_GRAVITE)
const String WIFI_SSID = "Sans Gravité";
const String WIFI_PASSWORD = "lovecraft";
#elif defined(COSTES)
const String WIFI_SSID = "Ouifi";
const String WIFI_PASSWORD = "28101948AC";
#elif defined(SUNNY)
const String WIFI_SSID = "Sunnyhome";
const String WIFI_PASSWORD = "20210616";
#else
const String WIFI_SSID = "LeNet";
const String WIFI_PASSWORD = "connectemoi";
#endif


// GENERAL PARAMETERS
const bool MASTER_DEBUG = true;

const bool OVERRIDE_NETWORK = true;
const long DELAY_BEFORE_RECONNECT_MS = 3000;
const long CONNECTION_TIMEOUT_MS = 10000;
const bool WIFI_DEBUG = false;

// OSC PARAMETERS
const String OSC_TARGET_IP = "192.168.0.8";
const long PING_TIMEOUT_MS = 1000;
const bool TARGET_IP_OVERRIDE = true;
const bool OSC_DEBUG = false;
const bool OSC_DEBUG_SEND = false;
const bool OSC_DEBUG_RECEIVE = false;

// BATTERY PARAMETERS
const long BATTERY_TIMOUT_MS = 5000;

// PLAYER PARAMETERS
const bool SEQUENCE_DEBUG = true;

// LED PARAMETERS
const bool LED_DEBUG = true;
const long LED_REFRESH_MS = 20;

// SERVO PARAMETERS
const bool SERVO_DEBUG = true;
const bool SERVO_ALLOW_UNSAFE_PINS = false;

// FEATHERWING PARAMETERS
const bool MOTORWING_DEBUG = true;

// ROOMBA PARAMETERS
const bool ROOMBA_DEBUG = true;
const long ROOMBA_TEXT_UPDATE_MS = 400;
