#pragma once
#include "Manager.h"
#include "../utils/EventBroadcaster.h"
#include "../communication/WifiManager.h"
#include "../communication/OSCManager.h"
#include "../sensors/SensorManager.h"
#include "../files/FileManager.h"
#include "../files/SequencePlayer.h"
#include "../leds/LedManager.h"
#include "../servo/ServoManager.h"
#include "../motorwing/MotorShield2Manager.h"
#include "../roomba/RoombaManager.h"


class PuppetMaster : public Manager
{
public:
    PuppetMaster();

    void init();
    void initManager(bool serialDebug);
    void checkComponents();
    void update();

    WifiManager wifiMgr;
    OSCManager oscMgr;
    SensorManager sensorMgr;
    
#ifdef HAS_SD_WING
    SequencePlayer player;
    FileManager fileMgr;
#endif
#ifdef NUM_LEDS
    LedManager ledMgr;
#endif
#ifdef NUM_SERVOS
    ServoManager servoMgr;
#endif
#ifdef HAS_MOTORWING
    MotorShield2Manager motorwingMgr;
#endif
#ifdef HAS_ROOMBA
    RoombaManager roombaMgr;
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
    void gotSensorValueEvent(const SensorValueEvent &e);

#ifdef HAS_SD_WING
    void gotFileEvent(const FileEvent &e);
    void gotPlayerEvent(const PlayerEvent &e);
#endif

#ifdef HAS_MOTORWING
    void gotStepperEvent(const StepperEvent &e);
#endif

#ifdef HAS_ROOMBA
    void gotRoombaValueEvent(const RoombaValueEvent &e);
#endif
    
};