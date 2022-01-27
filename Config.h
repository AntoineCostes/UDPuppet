#pragma once
#include <Arduino.h>
#include "Common.h"

////////////////////////////////////
// HARDWARE CONFIG - CHOOSE ONE FILE
////////////////////////////////////
#include "config/Teabox.h"
//#include "config/Test.h"
// #include "config/sansgravite/Ampoule.h"
// #include "config/sansgravite/Base.h"
// #include "config/sansgravite/Bobine.h"
// #include "config/sansgravite/Corbeille.h"
// #include "config/Roomba.h"
//#include "config/bluehouse/Biometry.h"
//#include "config/bluehouse/Showcase.h"

// NETWORK
const UDPuppetWifiCredentials WIFI_CREDENTIALS = CREDENTIALS[Networks::LENET];


// GENERAL PARAMETERS
// TODO ALLOW LED USE LED BUILTIN
const long BATTERY_TIMOUT_MS = 5000;

// NETWORK PARAMETERS
const bool OVERRIDE_WIFI_CREDS = true;
const long DELAY_BEFORE_RECONNECT_MS = 1000;
const long CONNECTION_TIMEOUT_MS = 30000;

// OSC PARAMETERS
const long PING_TIMEOUT_MS = 1000;
const bool TARGET_IP_OVERRIDE = true;
const String OSC_TARGET_IP = "192.168.0.1";

// LED PARAMETERS
// TODO DEBUG COLOR
const bool LED_WIFI_DEBUG_ALL = true;

// SERVO PARAMETERS
const bool SERVO_ALLOW_UNSAFE_PINS = true;


// DEBUG PARAMETERS
const bool MASTER_DEBUG = true;
const bool WIFI_DEBUG = true;
const bool OSC_DEBUG = true;
// TODO
//const bool OSC_DEBUG_SEND = false;
//const bool OSC_DEBUG_RECEIVE = false;
const bool LED_DEBUG = true;
const bool SERVO_DEBUG = true;
const bool SEQUENCE_DEBUG = true;
const bool MOTORWING_DEBUG = true;
const bool ROOMBA_DEBUG = true;
