#pragma once
#include <Arduino.h>

#include "config/Base.h"

const String WIFI_SSID = "LeNet";
const String WIFI_PASSWORD = "connectemoi";
const bool OVERRIDE_NETWORK = true;

const long PING_TIMEOUT_MS = 1000;
const long BATTERY_TIMOUT_MS = 5000;
const bool TARGET_IP_OVERRIDE = true;
const bool SERVO_SAFE_PINS = false; // TODO rename SERVO_ALLOW_UNSAFE_PINS

// serial debug
const bool MASTER_DEBUG =   true;
const bool WIFI_DEBUG =     false;
const bool OSC_DEBUG =      false;
const bool LED_DEBUG =      true;
const bool SERVO_DEBUG =    true;
const bool MOTORS_DEBUG =   true;
const bool SEQUENCE_DEBUG = true;
