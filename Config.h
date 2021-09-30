#pragma once
#include <Arduino.h>

enum BoardType
{
    HUZZAH32,
    HUZZAH8266 // not supported yet
};

//#include "config/sansgravite/Amoule.h"
#include "config/Roomba.h"

/*enum Network
{
    ANTOINE,
    COSTES,
    SANS_GRAVITE
};*/
#define COSTES // CHOOSE NETWORK


// GENERAL PARAMETERS
const bool MASTER_DEBUG = true;

// WIFI PARAMETERS
#ifdef SANS_GRAVITE
const String WIFI_SSID = "Sans Gravité";
const String WIFI_PASSWORD = "lovecraft";
#endif
#ifdef ANTOINE
const String WIFI_SSID = "LeNet";
const String WIFI_PASSWORD = "connectemoi";
#endif
#ifdef COSTES
const String WIFI_SSID = "Ouifi";
const String WIFI_PASSWORD = "28101948AC";
#endif
const bool OVERRIDE_NETWORK = true;
const long DELAY_BEFORE_RECONNECT_MS = 3000;
const long CONNECTION_TIMEOUT_MS = 10000;
const bool WIFI_DEBUG = false;

// OSC PARAMETERS
const String OSC_TARGET_IP = "192.168.43.14";
const long PING_TIMEOUT_MS = 1000;
const bool TARGET_IP_OVERRIDE = true;
const bool OSC_DEBUG = true;

// BATTERY PARAMETERS
const long BATTERY_TIMOUT_MS = 5000;

// PLAYER PARAMETERS
const bool SEQUENCE_DEBUG = true;

// LED PARAMETERS
const bool LED_DEBUG = true;
const long LED_REFRESH_MS = 20;

// SERVO PARAMETERS
const bool SERVO_DEBUG = true;
const int SERVO_MAX = 180;
const bool SERVO_ALLOW_UNSAFE_PINS = false;

// FEATHERWING PARAMETERS
const bool MOTORS_DEBUG = true; // TODO rename FEATHERWING

// ROOMBA PARAMETERS
const bool ROOMBA_DEBUG = true;
const long ROOMBA_TEXT_UPDATE_MS = 200;
