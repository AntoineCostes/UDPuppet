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
    NO_WIFI,
    LENET,
    COSTES,
    SANSGRAVITE,
    SUNSHINE
};

const UDPuppetWifiCredentials CREDENTIALS[5] = 
{
    {"", ""},
    {"LeNet", "connectemoi"},
    {"Ouifi", "28101948AC"},
    {"Sans Gravité", "lovecraft"},
    {"under the sunshine", "bibimbap"}
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
    bool useInSequences;
};

// TOWERPRO M90D [20, 170]
struct UDPuppetServo
{
    int pin;
    int min;
    int max;
    int start;
    bool inverse;
    bool isMultiServo;
    bool useInSequences;
};

// struct UDPuppetContinuousServo
// {
//     int pin;
//     bool inverse;
// };

struct UDPuppetRoomba
{
    int inPin;
    int outPin;
    int wakePin;
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