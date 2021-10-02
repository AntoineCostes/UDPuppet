#pragma once
#include "../common/Manager.h"
#include "WifiManager.h" // TODO remove
#include "../utils/EventBroadcaster.h"

#include <OSCMessage.h>
#include <WiFiUdp.h>
#include <ESPmDNS.h>

class OSCEvent
{
public:
    enum Type
    {
        CONNECTED,
        GOT_PING,
        PING_ALIVE,
        DEAD_PING,
        MDNS_ERROR,
        HANDSHAKE,
        COMMAND
    } type;
    OSCMessage *command;
    OSCEvent(Type type) : type(type), command(new OSCMessage()) {}
    OSCEvent(OSCMessage *command) : type(COMMAND), command(command) {}
};

class OSCManager : public Manager,
                   public EventBroadcaster<OSCEvent>
{
public:
    OSCManager(WifiManager *wifiMgr, String mDNSName);

    bool isConnected;
    String mDNSName;

    void initManager();
    void update();
    void connect();
    void disconnect();

    void yo();
    void pong();

    void sendMessage(OSCMessage &msg);
    void sendMessage(String address);

protected:
    WifiManager *wifi;
    WiFiUDP udp; //TODO pointer ?
    long lastSentPingMs;
    bool overrideTargetIp; // store dynamic target IP in flash

    void gotWifiEvent(const WifiEvent &e);
};