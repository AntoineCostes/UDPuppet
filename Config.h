#pragma once
#include <Arduino.h>
#include "UDPuppetClasses.h"

// NETWORK
const UDPuppetWifiCredentials WIFI_CREDENTIALS = CREDENTIALS[Networks::LENET];

////////////////////////////////////
// HARDWARE CONFIG - CHOOSE ONE FILE
////////////////////////////////////
//#include "config/sansgravite/Ampoule.h"
//#include "config/sansgravite/Base.h"
//#include "config/sansgravite/Bobine.h"
//#include "config/sansgravite/Corbeille.h"
//#include "config/sansgravite/Bouche.h"
//#include "config/Roomba.h"
// #include "config/Camembert.h"
// #include "config/Camembert_S3.h"
// #include "config/Camembert_8266.h"
//#include "config/Castafiore.h"
//#include "config/Castafiore_Bouton.h"
//#include "config/Castafiore_Anecdote.h"
//#include "config/Chantdrier.h"
//#include "config/Chantdrier_Jukebox.h"
//#include "config/Chantdrier_8266.h"
#include "config/Test.h"
//#include "config/Cartouche.h"

// GENERAL PARAMETERS
const int SETUP_DELAY_MS = 2000;

// WIFI PARAMETERS
const bool OVERRIDE_WIFI_CREDS = true;
const int DELAY_BEFORE_RECONNECT_MS = 3000;
const int CONNECTION_TIMEOUT_MS = 10000;

// OSC PARAMETERS
const String OSC_TARGET_IP = "";
const int PING_TIMEOUT_MS = 1000;
const bool TARGET_IP_OVERRIDE = true;

// BATTERY PARAMETERS
const int BATTERY_TIMEOUT_MS = 5000;
const int BATTERY_WINDOW_MS = 100;
const float BATTERY_LOW_VOLTAGE = 3.4f;

// PLAYER PARAMETERS
const int PLAYER_REFRESH_MS = 30;

// LED PARAMETERS
const int LED_REFRESH_MS = 20;

// SERVO PARAMETERS

// ROOMBA PARAMETERS
const int ROOMBA_TEXT_UPDATE_MS = 200;

// DEBUG PARAMETERS
const bool MASTER_DEBUG = true;
const bool BATTERY_DEBUG = true;
const bool WIFI_DEBUG = true;
const bool OSC_RECEIVE_DEBUG = false;
const bool OSC_SEND_DEBUG = false;
const bool FILES_DEBUG = true;
const bool SEQUENCE_DEBUG = true;
const bool LED_DEBUG = true;
const bool SERVO_DEBUG = false;
const bool MOTORWING_DEBUG = false;
const bool STEPPER_DEBUG = false;
const bool ROOMBA_DEBUG = true;
const bool BUTTON_DEBUG = false;