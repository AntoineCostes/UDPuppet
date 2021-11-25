#include "PuppetMaster.h"

// CLEAN
// passe sur les TODO et les FIXME
// separer stepper et DC
// OSC_DEBUG_SEND et OSC_DEBUG_REVEICE
// override
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
                               osc(&wifi, BOARD_NAME + " v" + "1.3.2")
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
    compLog("-------------------- " + osc.mDNSName + " --------------------");

    Manager::initManager();

    //std::set<int> reservedPins{};
    //Component::forbiddenPins.insert(reservedPins.begin(), reservedPins.end());

    // init managers and subscribe to their events
    managers.emplace_back(&wifi);
    wifi.initManager();
    managers.emplace_back(&osc);
    osc.initManager();
    wifi.addListener(std::bind(&PuppetMaster::gotWifiEvent, this, std::placeholders::_1));
    osc.addListener(std::bind(&PuppetMaster::gotOSCEvent, this, std::placeholders::_1));
    managers.emplace_back(&battery);
    battery.initManager();
    battery.addListener(std::bind(&PuppetMaster::gotBatteryEvent, this, std::placeholders::_1));

#ifdef HAS_SD_WING
    fileMgr.init();
    fileMgr.addListener(std::bind(&PuppetMaster::gotFileEvent, this, std::placeholders::_1));
    managers.emplace_back(&player);
    player.initManager();
    player.addListener(std::bind(&PuppetMaster::gotPlayerEvent, this, std::placeholders::_1));
#endif

#ifdef HAS_LED
    managers.emplace_back(&led);
    led.initManager();
#endif

#ifdef HAS_SERVO
    managers.emplace_back(&servo);
    servo.initManager();
#endif

#ifdef HAS_MOTORWING
    managers.emplace_back(&motorwing);
    motorwing.initManager();
    motorwing.addListener(std::bind(&PuppetMaster::gotStepperEvent, this, std::placeholders::_1));
#endif

#ifdef HAS_ROOMBA
    managers.emplace_back(&roomba);
    roomba.initManager();
#endif

    // TODO give this info on demand
    // compDebug("forbidden pins: ");
    // for (byte pin : Component::forbiddenPins)
    //     compDebug(String(pin));
}

void PuppetMaster::checkComponents()
{
#ifdef HAS_SD_WING
    OSCMessage msg("/sd");
    msg.add(BOARD_NAME.c_str());
    msg.add(fileMgr.sdIsDetected?1:0);
    osc.sendMessage(msg);
#endif

// CONFIX
// TODO advertise component parameters
/*
#ifdef AMPOULE
//TODO
#elif defined BASE
//TODO
#elif defined BOBINE
motorwing.stepperSetSpeed(0, 600.0f);
delay(1000);
motorwing.stepperSetSpeed(0, -600.0f);
delay(1000);
motorwing.stepperSetSpeed(0, 0.0f);
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
        if (!motorwing.handleCommand(command))
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
    //         led.setColor(0, 50, 0);
    //     else
    //         led.setColor(0, 0, 0);
    // }
}

void PuppetMaster::gotWifiEvent(const WifiEvent &e)
{
    switch (e.state)
    {
    case WifiConnectionState::CONNECTING:
        compDebug("connecting to wifi...");
    #ifdef HAS_LED
        led.setMode(LedStrip::LedMode::WORKING);
    #endif
        break;

    case WifiConnectionState::CONNECTED:
        compDebug("wifi connected !");
    #ifdef HAS_LED
        led.setMode(LedStrip::LedMode::STREAMING);
        led.setColor(0, 0, 50, 0);
        // led.toast(LedStrip::LedMode::READY, 1000); // probleme: ca reste vert si pas de stream
    #endif
        break;

    case WifiConnectionState::DISCONNECTED:
        compDebug("wifi lost !");
    #ifdef HAS_LED
        led.setMode(LedStrip::LedMode::ERROR);
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
        osc.yo();
        checkComponents();
        break;

    case OSCEvent::Type::DEAD_PING:
        // TODO advertise error
        break;

    case OSCEvent::Type::MDNS_ERROR:
        // TODO broadcast advertise error
        break;

    case OSCEvent::Type::PING_ALIVE: // not used
        //osc.pong();
        break;

    case OSCEvent::Type::HANDSHAKE:
        osc.yo();
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


#ifdef HAS_MOTORWING
void PuppetMaster::gotStepperEvent(const StepperEvent &e)
{
    if (!osc.isConnected)
        return;

    //compDebug("stepper event");

    OSCMessage msg("/stepper/pos"); //+String(e.index)));
    msg.add(BOARD_NAME.c_str());
    msg.add((int)e.position);
    msg.add((int)e.speed);
    msg.add((float)e.maxSpeed);
    osc.sendMessage(msg);
}
#endif

void PuppetMaster::gotBatteryEvent(const BatteryEvent &e)
{
    if (!osc.isConnected)
        return;

    OSCMessage msg("/battery");
    msg.add(BOARD_NAME.c_str());
    msg.add(e.normValue);
    msg.add(e.analogValue);
    msg.add(e.voltage);
    osc.sendMessage(msg);
}

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
        led.setColor((int)e.data[0], (int)e.data[1], (int)e.data[2]);
        servo.setServoRelativePosition(0, e.data[3] / 255.0f);
#elif defined BASE
        servo.setServoRelativePosition(1, e.data[0] / 255.0f); // foot
        servo.setServoRelativePosition(0, e.data[1] / 255.0f); // neck
        motorwing.stepperSetSpeedRel(0, e.data[2] / 127.0f - 1.0f);
#elif defined BOBINE
        motorwing.stepperSetSpeedRel(0, e.data[0] / 127.0f - 1.0f);
#elif defined CORBEILLE
        motorwing.dcRun(MotorShield2Manager::DCPort::M1, e.data[0] / 127.0f - 1.0f);
        motorwing.dcRun(MotorShield2Manager::DCPort::M2, e.data[1] / 127.0f - 1.0f);
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

// void PuppetMaster::commandFromOSCMessage(OSCMessage &command)
// {
//     char addr[32];
//     command.getAddress(addr, 1); //remove the first slash
//     String tc(addr);
//     int tcIndex = tc.indexOf('/');

//     int numData = command.size();
//     var *msgData = (var *)malloc(numData * sizeof(var));
//     int numUsedData = 0;

//     char tmpStr[32][32]; //contains potential strings

//     for (int i = 0; i < command.size(); i++)
//     {
//         switch (command.getType(i))
//         {
//         case 'i':
//             msgData[i].value.i = command.getInt(i);
//             msgData[i].type = 'i';
//             numUsedData++;
//             break;
//         case 'f':
//             msgData[i].value.f = command.getFloat(i);
//             msgData[i].type = 'f';
//             numUsedData++;
//             break;
//         case 's':
//             command.getString(i, tmpStr[i]);
//             msgData[i].value.s = tmpStr[i];
//             msgData[i].type = 's';
//             numUsedData++;
//             break;

//         default:
//             break;
//         }
//     }
//     // handle_command(tc.substring(0, tcIndex), tc.substring(tcIndex + 1), msgData, numUsedData);
//     compDebug(tc.substring(0, tcIndex));
//     compDebug(tc.substring(tcIndex + 1));
//     compDebug(String(numUsedData));

// }