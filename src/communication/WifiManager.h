#pragma once
#include "../common/Manager.h"
#include "../utils/EventBroadcaster.h"

#include <WiFi.h>

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


    void initManager();
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