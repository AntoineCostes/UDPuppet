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





/*
enum ErrorState
{
  Clear,
  ServoError
}
ErrorState errorState;

class BatteryEvent
{
public:
    enum Type { Level, Voltage, RawValue, CriticalLevel, Charging, TYPES_MAX};

    static const String eventNames[TYPES_MAX];

    BatteryEvent(Type type, float value) : type(type), value(value) {}
    Type type;
    float value;
};

class FileEvent
{
public:
    enum Type
    {
        UploadStart,
        UploadProgress,
        UploadComplete,
        UploadCancel
    };
    FileEvent(Type t, var data) : type(t), data(data) {}

    Type type;
    var data;
};

class TimerEvent
{
public:
    TimerEvent(Timer * timer) : timer(timer) {}
    Timer * timer;
};
*/