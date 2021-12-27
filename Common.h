#pragma once

enum BoardType
{
    HUZZAH32,
    THINGESP8266,
    HUZZAH8266 // not supported yet
};

struct UDPuppetWifiCredentials
{
    String ssid;
    String password;
};

enum NETWORKS
{  
    LENET,
    COSTES,
    SANSGRAVITE,
    SUNNY
};

const UDPuppetWifiCredentials CREDENTIALS[4] = 
{
    {"LeNet", "connectemoi"},
    {"Ouifi", "28101948AC"},
    {"Sans Gravité", "lovecraft"},
    {"Sunnyhome", "20210616"}
};

struct UDPuppetLedStrip
{
    String niceName;
    int pin;
    int numLeds;
    bool GRB;
};

struct UDPuppetServo
{
    String niceName;
    int pin;
    float minPosition;
    float maxPosition;
    float startPosition;
};

struct UDPuppetHCSR04
{
    String niceName;
    int triggerPin;
    int echoPin;
};