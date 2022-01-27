#pragma once
#include "../common/Manager.h"
#include "../utils/EventBroadcaster.h"

#ifdef ESP32
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#else
#error "This code is meant for ESP32 or ESP8266 only !"
#endif

enum WifiConnectionState
{
    DISCONNECTED,
    CONNECTING,
    CONNECTED,
    HOTSPOT
};

enum WifiError
{
    NONE,
    TIMEOUT,
    LOST
};

class WifiEvent
{
public:
    WifiConnectionState state;
    //WifiEvent(WifiConnectionState state, WifiError compError) : state(state), compError(compError) {}
    WifiEvent( WifiConnectionState state) : state(state) {}
};

class WifiManager : public Manager,
                    public EventBroadcaster<WifiEvent>
{
public:
    WifiManager();
    //~WifiManager(){Serial.println("delete WifiManager");}
    //~WifiManager() {}


    void initManager(bool serialDebug);
    void update();

    void connect();
    void disconnect();

    String getIP();
    String getMAC();

protected:
    unsigned long lastConnectTime;
    unsigned long lastDisconnectTime;

    void changeConnectionState(WifiConnectionState newState, WifiError compError = WifiError::NONE);
    WifiConnectionState connectionState;
    WifiError errorState;

};