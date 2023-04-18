#pragma once
#include <Arduino.h>
#include "UDPuppetClasses.h"

// Leds
#include <Adafruit_NeoPixel.h>
// Servos
#include <ESP32Servo.h>
#include <Adafruit_PWMServoDriver.h>
// motors
// #include <AccelStepper.h>
#include "src/AccelStepper_modified/src/AccelStepper.h"
#include <Adafruit_MotorShield.h>
// sensors
#include <Adafruit_VS1053.h>
// communication
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
// wifi
#include <WiFiUdp.h> 
#include <OSCMessage.h>
#include <ArduinoOTA.h>
#ifdef ESP32
#include <WiFi.h>
#include <ESPmDNS.h>
#elif defined (ESP8266)
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#else
#error "This code is meant for ESP32 or ESP8266 only !"
#endif



////////////////////////////////////
// HARDWARE CONFIG - CHOOSE ONE FILE
////////////////////////////////////
//#include "config/sansgravite/Ampoule.h"
//#include "config/sansgravite/Base.h"
//#include "config/sansgravite/Bobine.h"
//#include "config/sansgravite/Corbeille.h"
//#include "config/sansgravite/Bouche.h"
//#include "config/Roomba.h"
//#include "config/Camembert.h"
#include "config/Chantdrier.h"
// #include "config/Castafiore.h"
//#include "config/Test.h"

// NETWORK
const UDPuppetWifiCredentials WIFI_CREDENTIALS = CREDENTIALS[Networks::LENET];

// GENERAL PARAMETERS
const bool SETUP_DELAY_MS = 0;

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
const bool SERVO_ALLOW_UNSAFE_PINS = false; // 2, 4, 12, 13, 14, 15, 16, 17, 21, 22, 23, 25, 32

// ROOMBA PARAMETERS
const long ROOMBA_TEXT_UPDATE_MS = 200;

// DEBUG PARAMETERS
const bool MASTER_DEBUG = true;
const bool WIFI_DEBUG = false;
const bool OSC_DEBUG = false;
const bool FILES_DEBUG = false;
const bool SEQUENCE_DEBUG = false;
const bool LED_DEBUG = true;
const bool SERVO_DEBUG = true;
const bool MOTORWING_DEBUG = true;
const bool ROOMBA_DEBUG = true;