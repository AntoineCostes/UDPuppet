#include "PuppetMaster.h"


// TODO clear config
// TODO remove index
// TODO remove serialDebug for Component
// TODO local libs
// TODO filemanager server
// TODO OTA
// TODO send both on 12000 and specific port ?
// TODO inputPins & outPutPins PinManager
// analog refresh => merge battery as AnalogReader child ?

// CLEAN
// passe sur les TODO et les FIXME
// separer stepper et DC
// OSC_DEBUG_SEND et OSC_DEBUG_REVEICE
// rename init
// managers indépendants
// manager not a component
// osc sendMessage(address, data)
// events with var
// managers singletons, props vector or set
// component static bool registerPin(byte)
// Component Parameter event
// Component setParameter(String), setParameter(int), etc... ?
// compProgress()
// PuppetMaster singleton ?
// connectionstates inside class ? Wifi or Master ?

// make childClass DebugLedStrip with communication methods
// les managers ne devraient pas hériter de component car ils n'ont pas de paramètres

// TODO
// fix stepper
// include libs
// advertise SD not found
// Manager check(), error,
// servo init min max
// master check components + Parameter event to set Chataigne parameters
// OSC param override event
// OSC feedback when changing parameter
// clear manager components flash parameters
// SD ping : how to check if SD is still there ?
// debug as main parameter
// set color silent
// flash parameter button

PuppetMaster::PuppetMaster() : Manager("master"),
                               oscMgr(&wifiMgr, BOARD_NAME + " v" + "1.3.2")
{
    #ifdef BASE // Base uses pin 12 and 13

    #elif defined(ROOMBA) // Roomba uses pin 12
    Component::registerPin(LED_BUILTIN); 
    #else
    Component::registerPin(LED_BUILTIN); 
    Component::registerPin(12); // This pin has a pull-down resistor built into it, we recommend using it as an output only, or making sure that the pull-down is not affected during boot.
    #endif
    
    serialDebug = MASTER_DEBUG;
}

void PuppetMaster::init()
{
    initManager();
}

void PuppetMaster::initManager()
{
    compLog("");
    compLog("");
    compLog("");
    compLog("-------------------- " + oscMgr.mDNSName + " --------------------");

    Manager::initManager();

    //std::set<int> reservedPins{};
    //Component::forbiddenPins.insert(reservedPins.begin(), reservedPins.end());

    // init managers and subscribe to their events
    wifiMgr.initManager();
    wifiMgr.addListener(std::bind(&PuppetMaster::gotWifiEvent, this, std::placeholders::_1));
    managers.emplace_back(&wifiMgr);
    oscMgr.initManager();
    oscMgr.addListener(std::bind(&PuppetMaster::gotOSCEvent, this, std::placeholders::_1));
    managers.emplace_back(&oscMgr);
    sensorMgr.initManager();
    sensorMgr.addListener(std::bind(&PuppetMaster::gotBatteryEvent, this, std::placeholders::_1));
    sensorMgr.addListener(std::bind(&PuppetMaster::gotAnalogEvent, this, std::placeholders::_1));
    managers.emplace_back(&sensorMgr);

#ifdef HAS_SD_WING
    fileMgr.init();
    fileMgr.addListener(std::bind(&PuppetMaster::gotFileEvent, this, std::placeholders::_1));
    managers.emplace_back(&player);
    player.initManager();
    player.addListener(std::bind(&PuppetMaster::gotPlayerEvent, this, std::placeholders::_1));
#endif

#ifdef HAS_LED
    ledMgr.initManager();
    managers.emplace_back(&ledMgr);
#endif

#ifdef HAS_SERVO
    servoMgr.initManager();
    managers.emplace_back(&servoMgr);
#endif

#ifdef HAS_MOTORWING
    motorwingMgr.initManager();
    motorwingMgr.addListener(std::bind(&PuppetMaster::gotStepperEvent, this, std::placeholders::_1));
    managers.emplace_back(&motorwingMgr);
#endif

#ifdef HAS_ROOMBA
    roombaMgr.initManager();
    managers.emplace_back(&roombaMgr);
    roombaMgr.addListener(std::bind(&PuppetMaster::gotRoombaValueEvent, this, std::placeholders::_1));
#endif

    // TODO give this info on demand
    // compDebug("forbidden pins: ");
    // for (byte pin : Component::forbiddenPins)
    //     compDebug(String(pin));
}

void PuppetMaster::checkComponents()
{
#ifdef HAS_SD_WING
    OSCMessage msg( ("/" + BOARD_NAME + "/sd").c_str() );
    msg.add(fileMgr.sdIsDetected?1:0);
    oscMgr.sendMessage(msg);
#endif

// CONFIX
// TODO advertise component parameters
/*
#ifdef AMPOULE
//TODO
#elif defined BASE
//TODO
#elif defined BOBINE
motorwingMgr.stepperSetSpeed(0, 600.0f);
delay(1000);
motorwingMgr.stepperSetSpeed(0, -600.0f);
delay(1000);
motorwingMgr.stepperSetSpeed(0, 0.0f);
#elif defined CORBEILLE
//TODO
#endif
*/
}

void PuppetMaster::update()
{
    switch (BOARD_TYPE)
    {
    case HUZZAH32:
        break;

    default:
        compError("Unknown board type !");
        return;
    }

    for (auto const &mgr : managers)
    {
        mgr.get()->update();
    }
}

void PuppetMaster::sendCommand(OSCMessage &command)
{

    char buf[32];
    command.getAddress(buf);
    String address = String(buf);
    compDebug("command " + address);

    // TODO change with featherwing/stepper
#ifdef HAS_MOTORWING
    if (command.match("/stepper") || command.match("/dc"))
        if (!motorwingMgr.handleCommand(command))
            compError("motorwing could not handle command");
#endif

    int sepIndex = address.indexOf('/', 1);
    for (auto const &mgr : managers)
    {
        if (address.substring(1, sepIndex).equals(mgr.get()->name))
        {
            //compDebug("for " + mgr.get()->name);
            if (!mgr.get()->handleCommand(command))
                compError(mgr.get()->name + " could not handle command");
        }
    }

    // if (command.match("/debug"))
    // {
    //     if (command.getInt(0))
    //         ledMgr.setColor(0, 50, 0);
    //     else
    //         ledMgr.setColor(0, 0, 0);
    // }
}

void PuppetMaster::gotWifiEvent(const WifiEvent &e)
{
    switch (e.state)
    {
    case WifiConnectionState::CONNECTING:
        compDebug("connecting to wifi...");
    #ifdef HAS_LED
        ledMgr.setMode(LedStrip::LedMode::WORKING);
    #endif
        break;

    case WifiConnectionState::CONNECTED:
        compDebug("wifi connected !");
    #ifdef HAS_LED
        ledMgr.setMode(LedStrip::LedMode::STREAMING);
        ledMgr.setColor(0, 0, 50, 0);
        // ledMgr.toast(LedStrip::LedMode::READY, 1000); // probleme: ca reste vert si pas de stream
    #endif
        break;

    case WifiConnectionState::DISCONNECTED:
        compDebug("wifi lost !");
    #ifdef HAS_LED
        ledMgr.setMode(LedStrip::LedMode::ERROR);
    #endif
        break;

    default:
        compError("wifi state not handled");
        break;
    }
}

void PuppetMaster::gotOSCEvent(const OSCEvent &e)
{
    // compDebug("got OSC " + String(e.type));

    switch (e.type)
    {
    case OSCEvent::Type::CONNECTED:
        // TODO advertise error
        oscMgr.yo();
        checkComponents();
        break;

    case OSCEvent::Type::DEAD_PING:
        // TODO advertise error
        break;

    case OSCEvent::Type::MDNS_ERROR:
        // TODO broadcast advertise error
        break;

    case OSCEvent::Type::PING_ALIVE: // not used
        //oscMgr.pong();
        break;

    case OSCEvent::Type::HANDSHAKE:
        oscMgr.yo();
        checkComponents();
        break;

    case OSCEvent::Type::COMMAND:
        sendCommand(*e.command);
        break;

    default:
        compLog("OSCEvent :" + e.type);
        break;
    }
}

void PuppetMaster::gotBatteryEvent(const BatteryEvent &e)
{
    if (!oscMgr.isConnected)
        return;

    OSCMessage msg( ("/" + BOARD_NAME + "/battery").c_str() );
    msg.add(e.normValue);
    msg.add(e.analogValue);
    msg.add(e.voltage);
    oscMgr.sendMessage(msg);
}

void PuppetMaster::gotAnalogEvent(const AnalogEvent &e)
{
    if (!oscMgr.isConnected)
        return;

    OSCMessage msg( ("/" + BOARD_NAME + "/analog/" + e.niceName).c_str() );
    msg.add(e.normValue);
    msg.add(e.rawValue);
    oscMgr.sendMessage(msg);
}

#ifdef HAS_MOTORWING
void PuppetMaster::gotStepperEvent(const StepperEvent &e)
{
    if (!oscMgr.isConnected)
        return;

    //compDebug("stepper event");

    OSCMessage msg( ( "/" + BOARD_NAME + "/stepper/pos").c_str() ); //+String(e.index)));
    msg.add((int)e.position);
    msg.add((int)e.speed);
    msg.add((float)e.maxSpeed);
    oscMgr.sendMessage(msg);
}
#endif

#ifdef HAS_SD_WING
void PuppetMaster::gotPlayerEvent(const PlayerEvent &e)
{
    switch (e.type)
    {
    case PlayerEvent::NewFrame:
        break;

    case PlayerEvent::Stop:
        break;
    }

    //compDebug("Got Sequence Event : "+String(e.type));
    if (e.type == PlayerEvent::NewFrame)
    {
//compDebug("Got new frame ! "+String((int)e.data[0]));
#ifdef AMPOULE
        ledMgr.setColor((int)e.data[0], (int)e.data[1], (int)e.data[2]);
        servoMgr.setServoRelativePosition(0, e.data[3] / 255.0f);
#elif defined BASE
        servoMgr.setServoRelativePosition(1, e.data[0] / 255.0f); // foot
        servoMgr.setServoRelativePosition(0, e.data[1] / 255.0f); // neck
        motorwingMgr.stepperSetSpeedRel(0, e.data[2] / 127.0f - 1.0f);
#elif defined BOBINE
        motorwingMgr.stepperSetSpeedRel(0, e.data[0] / 127.0f - 1.0f);
#elif defined CORBEILLE
        motorwingMgr.dcRun(MotorShield2Manager::DCPort::M1, e.data[0] / 127.0f - 1.0f);
        motorwingMgr.dcRun(MotorShield2Manager::DCPort::M2, e.data[1] / 127.0f - 1.0f);
#endif
    }
}

void PuppetMaster::gotFileEvent(const FileEvent &e)
{
    // TODO useful ?
    if (e.type == FileEvent::Type::SD_OK || FileEvent::Type::SD_ERROR)
        checkComponents();


}
#endif // HAS_SD_WING

#ifdef HAS_MOTORWING
void PuppetMaster::gotStepperEvent(const StepperEvent &e)
{
    if (!oscMgr.isConnected)
        return;

    //compDebug("stepper event");

    OSCMessage msg( ( "/" + BOARD_NAME + "/stepper/pos").c_str() ); //+String(e.index)));
    msg.add((int)e.position);
    msg.add((int)e.speed);
    msg.add((float)e.maxSpeed);
    oscMgr.sendMessage(msg);
}
#endif

#ifdef HAS_ROOMBA
void PuppetMaster::gotRoombaValueEvent(const RoombaValueEvent &e)
{
    if (!oscMgr.isConnected)
        return;

    compDebug("roomba event");

    // OSCMessage msg( ( "/" + BOARD_NAME + "/stepper/pos").c_str() ); //+String(e.index)));
    // msg.add((int)e.position);
    // msg.add((int)e.speed);
    // msg.add((float)e.maxSpeed);
    // oscMgr.sendMessage(msg);
}
#endif
