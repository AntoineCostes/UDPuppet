#pragma once

// general
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <string.h>
#include <iterator>
#include <functional>

// esp32 specific stuff
#ifdef ESP32
    #include <SPIFFS.h>
    #include <Preferences.h>
    #include "Adafruit_MAX1704X.h"
#endif

// leds
#ifdef NUM_STRIPS
    #include <Adafruit_NeoPixel.h>
#endif

// servos
#ifdef NUM_SERVOS
    #include <Adafruit_PWMServoDriver.h>
    #ifdef ESP32
        #include <ESP32Servo.h>
    #else
        #include <Servo.h>
    #endif
#endif

// motors
#ifdef HAS_MOTORWING
    #include <Adafruit_MotorShield.h>
#endif
#ifdef HAS_STEPPER_DRIVER
    // #include <AccelStepper.h>
    #include "src/AccelStepper_modified/src/AccelStepper.h"
#endif

// audio
#ifdef HAS_MUSICMAKER
    #include <Adafruit_VS1053.h>
#endif

// files
#include <SD.h>
#include <FS.h>

// communication
#include <SoftwareSerial.h>
#include <SPI.h>

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
#endif

// webserver
#include <ESPAsyncWebServer.h>
#ifdef ESP32
    #include <AsyncTCP.h>
#elif defined (ESP8266)
    #include <ESPAsyncTCP.h>
#endif

