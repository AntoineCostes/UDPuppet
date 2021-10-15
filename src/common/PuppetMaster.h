#pragma once
#include "Manager.h"
#include "../utils/EventBroadcaster.h"
#include "../communication/WifiManager.h"
#include "../communication/OSCManager.h"
#include "../sensors/BatteryManager.h"
#include "../sensors/AnalogManager.h"
#include "../files/FileManager.h"
#include "../files/SequencePlayer.h"
#include "../leds/LedManager.h"
#include "../motors/ServoManager.h"
#include "../motorwing/MotorShield2Manager.h"
#include "../roomba/RoombaManager.h"


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
    BatteryManager battery;
    AnalogManager analog;
    
#ifdef HAS_SD_WING
    SequencePlayer player;
    FileManager fileMgr;
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

protected:
    std::vector<std::unique_ptr<Manager>> managers; // TODO useful ? set instead of vector

    bool registerManager(Manager *mgr, std::set<int> reservedPins);

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

    void gotWifiEvent(const WifiEvent &e);
    void gotOSCEvent(const OSCEvent &e);
    void gotBatteryEvent(const BatteryEvent &e);
    void gotAnalogEvent(const AnalogEvent &e);

#ifdef HAS_SD_WING
    void gotFileEvent(const FileEvent &e);
    void gotPlayerEvent(const PlayerEvent &e);
#endif

#ifdef HAS_MOTORWING
    void gotStepperEvent(const StepperEvent &e);
#endif

#ifdef HAS_ROOMBA
#endif
    
};