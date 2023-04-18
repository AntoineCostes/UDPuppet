#pragma once

enum BoardType
{
    HUZZAH32,
    THINGESP8266,
    HUZZAH8266 // not supported yet
};

///////////////////
// WIFI CREDENTIALS
///////////////////
struct UDPuppetWifiCredentials
{
    String ssid;
    String password;
};

enum Networks
{  
    LENET,
    COSTES,
    SANSGRAVITE
};

const UDPuppetWifiCredentials CREDENTIALS[3] = 
{
    {"LeNet", "connectemoi"},
    {"Ouifi", "28101948AC"},
    {"Sans Gravité", "lovecraft"}
};

///////////////////
// COMPONENTS
///////////////////
struct UDPuppetLedStrip
{
    int pin;
    int numLeds;
    bool GRB;
    bool wifiDebug;
};

struct UDPuppetServo
{
    int pin;
    int min;
    int max;
    int start;
    bool inverse;
};

struct UDPuppetButton
{
    int pin;
    int longPressMs;
};

struct UDPuppetHCSR04
{
    int triggerPin;
    int echoPin;
    boolean active;
};