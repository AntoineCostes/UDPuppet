#pragma once
#include "Manager.h"
#include "../utils/EventBroadcaster.h"
#include "../communication/WifiManager.h"
#include "../communication/OSCManager.h"
#include "../files/FileManager.h"
#include "../files/WebServerManager.h"
#include "../files/SequencePlayer.h"
#include "../leds/LedManager.h"
#include "../motors/ServoManager.h"
#include "../motorwing/MotorShield2Manager.h"
#include "../roomba/RoombaManager.h"
#include "../audio/MusicMakerManager.h"
#ifdef ESP32
#include "../sensors/BatteryManager.h"
#endif

#ifdef ESP32
#include <ESPmDNS.h>
#elif defined (ESP8266)
#include <ESP8266mDNS.h>
#endif

#ifdef HAS_MULTISERVO
#ifndef HAS_SERVO
#define HAS_SERVO
#endif
#endif

class PuppetMaster : public Manager
{
public:
    PuppetMaster();

    void init();
    void initManager();
    void advertiseSequences();
    void checkComponents();
    void update();
    static void sendDebugMsg(String componentName, String msg); // compilation problem with including PuppetMaster in FileManager

    WifiManager wifi;
    OSCManager osc;
    SequencePlayer player;
    FileManager fileMgr;
    WebServerManager web;
    
    #ifdef ESP32
    BatteryManager battery;
    #endif

#ifdef HAS_LED
    LedManager led;
#endif
#ifdef HAS_SERVO
    ServoManager servo;
#endif
#ifdef HAS_MOTORWING
    MotorShield2Manager motorwing;
#endif
#ifdef HAS_ROOMBA
    RoombaManager roomba;
#endif
#ifdef HAS_MUSICMAKER
    MusicMakerManager music;
#endif

protected:
    std::vector<std::unique_ptr<Manager>> managers; // TODO useful ? set instead of vector

    bool registerManager(Manager *mgr, std::set<int> reservedPins);

    void sendCommand(OSCMessage &command);

    String firmwareVersion;

    void launchSequence(String sequenceName);

    enum State
    {
        WAITING,
        READY,
        WORKING,
        ERROR,
        STREAMING
    } mode;

    void gotWifiEvent(const WifiEvent &e);
    void gotOSCEvent(const OSCEvent &e);
    void gotFileEvent(const FileEvent &e);
    void gotPlayerEvent(const PlayerEvent &e);
    #ifdef ESP32
    void gotBatteryEvent(const BatteryEvent &e);
    #endif

#ifdef HAS_MOTORWING
    void gotStepperEvent(const StepperEvent &e);
#endif

#ifdef HAS_ROOMBA
#endif
    
};