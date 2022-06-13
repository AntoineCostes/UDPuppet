#pragma once
#include <Arduino.h>

// NETWORKS - DEFAULT TO LENET
#define SANS_GRAVITE
//#define JEREM
//#define COSTES

// HARDWARE - SELECT ONE
//#define AMPOULE
//#define BASE
//#define BOBINE
//#define CORBEILLE
#define BOUCHE
//#define ROOMBA
//#define TEST
//#define CAMEMBERT
//#define CHANTDRIER
//#define CASTAFIORE

// WIFI PARAMETERS
#if defined(SANS_GRAVITE)
const String WIFI_SSID = "Sans Gravité";
const String WIFI_PASSWORD = "lovecraft";
#elif defined(COSTES)
const String WIFI_SSID = "Ouifi";
const String WIFI_PASSWORD = "28101948AC";
#elif defined(JEREM)
const String WIFI_SSID = "TP-LINK_AP_E97865";
const String WIFI_PASSWORD = "";
#else
const String WIFI_SSID = "LeNet";
const String WIFI_PASSWORD = "connectemoi";
#endif

enum BoardType
{
    HUZZAH32,
    HUZZAH8266
};

#if defined(AMPOULE)
#include "config/sansgravite/Ampoule.h"
#elif defined(BASE)
#include "config/sansgravite/Base.h"
#elif defined(BOBINE)
#include "config/sansgravite/Bobine.h"
#elif defined(CORBEILLE)
#include "config/sansgravite/Corbeille.h"
#elif defined(BOUCHE)
#include "config/sansgravite/Bouche.h"
#elif defined(TEST)
#include "config/sansgravite/Test.h"
#elif defined(ROOMBA)
#include "config/Roomba.h"
#elif defined(CAMEMBERT)
#include "config/Camembert.h"
#elif defined(CHANTDRIER)
#include "config/Chantdrier.h"
#elif defined(CASTAFIORE)
#include "config/Castafiore.h"
#endif


// GENERAL PARAMETERS
const bool MASTER_DEBUG = true;

const bool OVERRIDE_NETWORK = true;
const long DELAY_BEFORE_RECONNECT_MS = 3000;
const long CONNECTION_TIMEOUT_MS = 10000;
const bool WIFI_DEBUG = false;

// OSC PARAMETERS
const String OSC_TARGET_IP = "";
const long PING_TIMEOUT_MS = 1000;
const bool TARGET_IP_OVERRIDE = true;
const bool OSC_DEBUG = false;

// BATTERY PARAMETERS
const long BATTERY_TIMOUT_MS = 5000;

// PLAYER PARAMETERS
const bool SEQUENCE_DEBUG = true;

// LED PARAMETERS
const bool LED_DEBUG = true;
const long LED_REFRESH_MS = 20;

// SERVO PARAMETERS
const bool SERVO_DEBUG = true;
const bool SERVO_ALLOW_UNSAFE_PINS = true;

// FEATHERWING PARAMETERS
const bool MOTORWING_DEBUG = true;

// ROOMBA PARAMETERS
const bool ROOMBA_DEBUG = true;
const long ROOMBA_TEXT_UPDATE_MS = 200;
