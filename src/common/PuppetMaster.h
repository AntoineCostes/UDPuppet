#pragma once
#include "Manager.h"
#include "../utils/EventBroadcaster.h"

#include "../communication/WifiManager.h"
#include "../communication/OSCManager.h"
#include "../files/WebServerManager.h"

#include "../files/FileManager.h"
#include "../files/SequencePlayer.h"

#ifdef NUM_STRIPS
#include "../leds/LedManager.h"
#endif
#ifdef NUM_SERVOS
#include "../servos/ServoManager.h"
#endif
#ifdef HAS_MOTORWING
#include "../motorwing/MotorShield2Manager.h"
#endif
#ifdef HAS_STEPPER_DRIVER
#include "../motorwing/StepperManager.h"
#endif
#ifdef HAS_ROOMBA
#include "../roomba/RoombaManager.h"
#endif
#ifdef HAS_MUSICMAKER
#include "../audio/MusicMakerManager.h"
#endif
#include "../sensors/ButtonManager.h"
#ifdef ESP32
#include "../sensors/BatteryManager.h"
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

    ButtonManager button;
    
#ifdef ESP32
    BatteryManager battery;
#endif

#ifdef NUM_STRIPS
    LedManager led;
#endif
#ifdef NUM_SERVOS
    ServoManager servo;
#endif
#ifdef HAS_MOTORWING
    MotorShield2Manager motorwing;
#endif
#ifdef HAS_STEPPER_DRIVER
    StepperManager stepperdriver;
#endif
#ifdef HAS_ROOMBA
    RoombaManager roomba;
#endif
#ifdef HAS_MUSICMAKER
    MusicMakerManager musicmaker;
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

    void gotPlayerEvent(const PlayerEvent &e);

    void gotButtonEvent(const ButtonEvent &e);
    
    void gotFileEvent(const FileEvent &e);

#ifdef ESP32
    void gotBatteryEvent(const BatteryEvent &e);
#endif

#ifdef HAS_MOTORWING
    void gotStepperEvent(const StepperEvent &e);
#endif

#ifdef HAS_STEPPER_DRIVER
    void gotStepperEvent(const StepperEvent2 &e);
#endif

#ifdef HAS_ROOMBA
    void gotRoombaValueEvent(const RoombaValueEvent &e);
#endif

#if defined(BUTTON_JUKEBOX) || defined(CONTINUE_PLAYING)
    int trackIndex;
#endif
    
};