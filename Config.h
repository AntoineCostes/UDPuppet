#pragma once
#include <Arduino.h>
#include "UDPuppetClasses.h"

////////////////////////////////////
// HARDWARE CONFIG - CHOOSE ONE FILE
////////////////////////////////////
//#include "config/sansgravite/Ampoule.h"
//#include "config/sansgravite/Base.h"
//#include "config/sansgravite/Bobine.h"
//#include "config/sansgravite/Corbeille.h"
//#include "config/sansgravite/Bouche.h"
//#include "config/sansgravite/Test.h"
//#include "config/Roomba.h"
//#include "config/Camembert.h"
//#include "config/Chantdrier.h"
#include "config/Castafiore.h"

// NETWORK
const UDPuppetWifiCredentials WIFI_CREDENTIALS = CREDENTIALS[Networks::LENET];


// GENERAL PARAMETERS
const bool MASTER_DEBUG = true;

// WIFI PARAMETERS
const bool OVERRIDE_WIFI_CREDS = true;
const long DELAY_BEFORE_RECONNECT_MS = 3000;
const long CONNECTION_TIMEOUT_MS = 10000;

// OSC PARAMETERS
const String OSC_TARGET_IP = "192.168.0.1";
const long PING_TIMEOUT_MS = 1000;
const bool TARGET_IP_OVERRIDE = true;

// BATTERY PARAMETERS
const long BATTERY_TIMOUT_MS = 5000;

// LED PARAMETERS
const long LED_REFRESH_MS = 20;

// SERVO PARAMETERS
const bool SERVO_ALLOW_UNSAFE_PINS = true;

// ROOMBA PARAMETERS
const long ROOMBA_TEXT_UPDATE_MS = 200;

// DEBUG PARAMETERS
const bool WIFI_DEBUG = false;
const bool OSC_DEBUG = false;
const bool FILES_DEBUG = false;
const bool SEQUENCE_DEBUG = false;
const bool LED_DEBUG = true;
const bool SERVO_DEBUG = true;
const bool MOTORWING_DEBUG = true;
const bool ROOMBA_DEBUG = true;