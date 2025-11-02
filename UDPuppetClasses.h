#pragma once

enum BoardType
{
    HUZZAH32,
    HUZZAH8266,
    THINGESP8266,
    HUZZAH32_S3,
    XIAO_C3,
    XIAO_S3
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
    SUNSHINE,
    AKINDOFMAGIC,
    SCHBAFFE
};

const UDPuppetWifiCredentials CREDENTIALS[7] = 
{
    {"", ""},
    {"LeNet", "connectemoi"},
    {"Ouifi", "28101948AC"},
    {"Sans Gravité", "lovecraft"},
    {"under the sunshine", "bibimbap"},
    {"akindofmagic", "akindofmagic"},
    {"Schbaffe", "Papillon"}
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

struct UDPuppetButtonBehavior
{
    bool clearOnPressed;
    bool playSequencesOnShort;
    bool cancelSoundOnLongPress;
    bool enableHotspotOnLong;
};

struct UDPuppetButton
{
    int pin;
    int shortPressMs;
    int longPressMs;
    UDPuppetButtonBehavior behavior;
};

struct UDPuppetHCSR04
{
    String niceName;
    int triggerPin;
    int echoPin;
    boolean active;
};
