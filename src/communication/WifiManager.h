#pragma once
#include "../common/Manager.h"
#include "../utils/EventBroadcaster.h"

class WifiEvent
{
public:
    enum ConnectionState
    {
        DISCONNECTED,
        CONNECTING,
        CONNECTED,
        HOTSPOT
    } state;
    //WifiEvent(WifiEvent::ConnectionState state, WifiManager::Error compError) : state(state), compError(compError) {}
    WifiEvent(ConnectionState state) : state(state) {}
};


class WifiManager : public Manager,
                    public EventBroadcaster<WifiEvent>
{
public:
    WifiManager();
    //~WifiManager(){Serial.println("delete WifiManager");}
    //~WifiManager() {}


    void initManager() override;
    void update() override;

    void connect();
    void disconnect();

    String getIP();
    String getMAC();

    enum Error
    {
        NONE,
        TIMEOUT,
        LOST
    };

protected:
    unsigned long lastConnectTime;
    unsigned long lastDisconnectTime;

    void changeConnectionState(WifiEvent::ConnectionState newState, WifiManager::Error compError = WifiManager::Error::NONE);
    WifiEvent::ConnectionState connectionState;
    WifiManager::Error errorState;
};
