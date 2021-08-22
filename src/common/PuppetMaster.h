#pragma once
#include "Manager.h"
#include "../utils/EventBroadcaster.h"
#include "../sensors/BatteryManager.h"
#include "../communication/WifiManager.h"
#include "../communication/OSCManager.h"
#include "../leds/LedManager.h"
#include "../motors/ServoManager.h"
#include "../motorwing/MotorShield2Manager.h"
#include "../files/FileManager.h"
#include "../files/SequencePlayer.h"

class PuppetMaster : public Manager
{
public:
    PuppetMaster();

    void init();
    void initManager();
    void checkComponents();
    void update();

    WifiManager wifi;
    OSCManager osc;
    LedManager led;
    ServoManager servo;
    MotorShield2Manager motorwing;
    BatteryManager battery;
    SequencePlayer player;

    FileManager fileMgr;

protected:
    std::vector<std::unique_ptr<Manager>> managers; // TODO useful ? set instead of vector

    bool registerManager(Manager *mgr, std::set<int> reservedPins);

    void gotFileEvent(const FileEvent &e);
    void gotWifiEvent(const WifiEvent &e);
    void gotOSCEvent(const OSCEvent &e);
    void gotStepperEvent(const StepperEvent &e);
    void gotBatteryEvent(const BatteryEvent &e);
    void gotPlayerEvent(const PlayerEvent &e);
    
    void sendCommand(OSCMessage &command);

    String firmwareVersion;

    enum State
    {
        WAITING,
        READY,
        WORKING,
        ERROR,
        STREAMING
    } mode;
};