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
                               osc(&wifi),
                               firmwareVersion("1.4.7")
{
    #ifdef BASE // Base uses pin 12 and 13
    // don't register
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
    compLog("-------------- " + BOARD_NAME + " v" + firmwareVersion + " " + __DATE__ +" ----------------");

    Manager::initManager();

    // init managers and subscribe to their events
    managers.emplace_back(&wifi);
    wifi.initManager();
    managers.emplace_back(&osc);
    osc.initManager();
    wifi.addListener(std::bind(&PuppetMaster::gotWifiEvent, this, std::placeholders::_1));
    osc.addListener(std::bind(&PuppetMaster::gotOSCEvent, this, std::placeholders::_1));

#ifdef ESP32
    managers.emplace_back(&battery);
    battery.initManager();
    battery.addListener(std::bind(&PuppetMaster::gotBatteryEvent, this, std::placeholders::_1));
#endif

    fileMgr.init();
    managers.emplace_back(&player);
    player.initManager();
    player.addListener(std::bind(&PuppetMaster::gotPlayerEvent, this, std::placeholders::_1));

    managers.emplace_back(&web);
    web.initManager();
    web.addListener(std::bind(&PuppetMaster::gotFileEvent, this, std::placeholders::_1));

    managers.emplace_back(&button);
    button.initManager();
    button.addListener(std::bind(&PuppetMaster::gotButtonEvent, this, std::placeholders::_1));

#ifdef NUM_LEDS
    managers.emplace_back(&led);
    led.initManager();
#endif

#ifdef NUM_SERVOS
    managers.emplace_back(&servo);
    servo.initManager();
#endif

#ifdef HAS_MOTORWING
    managers.emplace_back(&motorwing);
    motorwing.initManager();
    motorwing.addListener(std::bind(&PuppetMaster::gotStepperEvent, this, std::placeholders::_1));
#endif

#ifdef HAS_STEPPER_DRIVER
    managers.emplace_back(&stepperdriver);
    stepperdriver.initManager();
    stepperdriver.addListener(std::bind(&PuppetMaster::gotStepperEvent, this, std::placeholders::_1));
#endif

#ifdef HAS_ROOMBA
    managers.emplace_back(&roomba);
    roomba.initManager();
#endif

#ifdef HAS_MUSICMAKER
    managers.emplace_back(&music);
    music.initManager();
#endif

    // TODO give this info on demand
    // compDebug("forbidden pins: ");
    // for (int pin : Component::forbiddenPins)
    //     compDebug(String(pin));
    
}

void PuppetMaster::advertiseSequences()
{
    OSCMessage msg("/sequences");
    msg.add(BOARD_NAME.c_str());
    for (auto seq : fileMgr.sequences) msg.add(seq.c_str());
    osc.sendMessage(msg);
}

void PuppetMaster::checkComponents()
{
 // TODO make FileManager singleton
#ifdef HAS_ADALOGGER_WING
    OSCMessage msg("/sd");
    msg.add(BOARD_NAME.c_str());
    msg.add(fileMgr.sdIsDetected?1:0);
    osc.sendMessage(msg);
#endif

#ifdef HAS_MUSICMAKER
    OSCMessage msg3("/tracks");
    msg3.add(BOARD_NAME.c_str());
    for (auto seq : music.tracks) msg3.add(seq.c_str());
    osc.sendMessage(msg3);

    OSCMessage msg4("/volume");
    msg4.add(BOARD_NAME.c_str());
    msg4.add(music.getVolume());
    osc.sendMessage(msg4);
    
    OSCMessage msg("/sd");
    msg.add(BOARD_NAME.c_str());
    msg.add(music.isReady()?1:0);
    osc.sendMessage(msg);
#endif

    advertiseSequences();

// FIXME /dc/0
#ifdef HAS_MOTORWING
    for (auto &pair : motorwing.dcMotors)
    {
        String addr = "/dc/maxspeed/"+String(int(pair.first));
        OSCMessage msg(addr.c_str());
        msg.add(BOARD_NAME.c_str());
        msg.add(pair.second->getMaxSpeed());
        osc.sendMessage(msg);
    }
    
    for(std::size_t i = 0; i < motorwing.steppers.size(); ++i)
    //for (auto &stepper : motorwing.steppers)
    {
        compDebug("check stepper");
        String addr = "/stepper/"+String(i)+"/maxspeed";
        OSCMessage msg(addr.c_str());
        msg.add(BOARD_NAME.c_str());
        msg.add(motorwing.steppers[i]->maxSpeed());
        osc.sendMessage(msg);
        
       // FIXME AccelStepper ne donne pas acces à l'acceleration, modifier la classe ?
        // String addr2 = "/stepper/"+String(int(pair.first)+"/acceleration");
        // OSCMessage msg2(addr.c_str());
        // msg2.add(BOARD_NAME.c_str());
        // msg2.add(pair.second->acceleration());  // FIXME acceleration la même pour AccelStepper et StepperMotor ou pas ?
        // osc.sendMessage(msg2);
        
        // compDebug(String(pair.second->acceleration()));
    }
#endif


#ifdef HAS_STEPPER_DRIVER
    
    for(std::size_t i = 0; i < stepperdriver.steppers.size(); ++i)
    {
        compDebug("check stepper");
        String addr = "/stepper/"+String(i)+"/maxspeed";
        OSCMessage msg(addr.c_str());
        msg.add(BOARD_NAME.c_str());
        msg.add(stepperdriver.steppers[i]->maxSpeed());
        osc.sendMessage(msg);
        
       // FIXME AccelStepper ne donne pas acces à l'acceleration, modifier la classe ?
        // String addr2 = "/stepper/"+String(int(pair.first)+"/acceleration");
        // OSCMessage msg2(addr.c_str());
        // msg2.add(BOARD_NAME.c_str());
        // msg2.add(pair.second->acceleration());  // FIXME acceleration la même pour AccelStepper et StepperMotor ou pas ?
        // osc.sendMessage(msg2);
        
        // compDebug(String(pair.second->acceleration()));
    }
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
        
    case HUZZAH8266:
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

void PuppetMaster::sendDebugMsg(String componentName, String msg)
{
    // ifdef SERIAL_DEBUG
    Serial.println("["+componentName+"] "+msg);
    // ifdef OSC_DEBUG
    // static osc send
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

    if (command.match("/dc/run"))
    {
        float value = abs(command.getFloat(1));
        if (command.getInt(0) == 2)
            for (int i = 0; i < 6 ; i++)
                led.setColor(0, i, 0, value*255, 0);
                
        if (command.getInt(0) == 1)
            for (int i = 6; i < 12 ; i++)
                led.setColor(0, i, 0, 0, value*255);
    }            
#endif

    int sepIndex = address.indexOf('/', 1);
    for (auto const &mgr : managers)
    {
        if (address.substring(1, sepIndex).equals(mgr.get()->name))
        {
            if (!mgr.get()->handleCommand(command))
                compError(mgr.get()->name + " could not handle command");
        }
    }
    
    if (command.match("/play"))
    {
        // for (int i = 0; i < NUM_SERVOS; i++) servo.servoGoToStart(i);

        char str[32];
        command.getString(0, str);
        launchSequence(String(str));
    } 

    if (command.match("/delete"))
    {
        char str[32];
        command.getString(0, str);
        fileMgr.deleteFileIfExists(String(str)+".dat");
    } 

    // if (command.match("/debug"))
    // {
    //     if (command.getInt(0))
    //         led.setColor(0, 50, 0);
    //     else
    //         led.setColor(0, 0, 0);
    // }
}

void PuppetMaster::launchSequence(String sequenceName)
{
    // TODO get File from fileManager and give it to player ?
    player.playSequence(sequenceName);
    #ifdef HAS_MUSICMAKER
    music.play(sequenceName+".mp3");
    #endif 
}

void PuppetMaster::gotWifiEvent(const WifiEvent &e)
{
    switch (e.state)
    {
    case WifiConnectionState::CONNECTING:
        compDebug("connecting to wifi...");
    #ifdef NUM_LEDS
        led.setMode(LedStrip::LedMode::WORKING);
    #endif
        break;

    case WifiConnectionState::CONNECTED:
        compDebug("wifi connected !");
        

        compLog("creating mDNS instance: " + BOARD_NAME);// BOARD_NAME+ " v" + "1.3.5"));
        if (MDNS.begin(BOARD_NAME.c_str()))
        {
            MDNS.addService("_osc", "_udp", OSC_LISTENING_PORT);
            MDNS.addService("_http", "_tcp", 80);
            compDebug("OSC Zeroconf service added sucessfully !");
        }
        else
        {
            compError("could not set up mDNS instance");
        }

        // FIXME wifiDebug
    #ifdef NUM_LEDS
        led.setMode(LedStrip::LedMode::READY);
        //led.setColor(0, 0, 50, 0);
        // led.toast(LedStrip::LedMode::READY, 1000); // probleme: ca reste vert si pas de stream
    #endif
        #ifdef NUM_SERVOS
        for (int i = 0; i < NUM_SERVOS; i++) servo.servoGoTo(i, 0.0f);
        delay(500);
        for (int i = 0; i < NUM_SERVOS; i++) servo.servoGoTo(i, 0.5f);
        delay(500);
        for (int i = 0; i < NUM_SERVOS; i++) servo.servoGoToStart(i);
        #endif
        web.initServer();
        
        break;

    case WifiConnectionState::DISCONNECTED:
        compDebug("wifi lost !");
    #ifdef NUM_LEDS
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
    //compDebug("got OSC " + String(e.type));

    switch (e.type)
    {
    case OSCEvent::Type::CONNECTED:
        // TODO advertise error
        osc.yo(firmwareVersion);
        checkComponents();
        break;

    case OSCEvent::Type::DEAD_PING:
        // TODO advertise error
        break;

    case OSCEvent::Type::PING_ALIVE: // not used
        //osc.pong();
        break;

    case OSCEvent::Type::HANDSHAKE:
        osc.yo(firmwareVersion);
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

void PuppetMaster::gotButtonEvent(const ButtonEvent &e)
{
    compDebug("got button " + String(e.type));

    switch (e.type)
    {
#ifdef CASTAFIORE_BUTTON
    case ButtonEvent::Type::PRESSED:
        music.stop();
        player.stopPlaying();
        servo.servoGoTo(0, 0.0f);
        break;

    case ButtonEvent::Type::RELASED_SHORT:
        launchSequence(fileMgr.sequences[trackIndex]);
        trackIndex++;
        if (trackIndex >= fileMgr.sequences.size()) trackIndex = 0;
        compLog("track index :" + trackIndex);
        break;
#endif

    default:
        compLog("ButtonEvent :" + e.type);
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

#ifdef HAS_STEPPER_DRIVER
void PuppetMaster::gotStepperEvent(const StepperEvent2 &e)
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

#ifdef ESP32
void PuppetMaster::gotBatteryEvent(const BatteryEvent &e)
{
    if (!osc.isConnected)
        return;

    OSCMessage msg("/battery");
    msg.add(BOARD_NAME.c_str());
    msg.add(e.level);
    msg.add(e.voltage);
    msg.add(e.analogValue);
    osc.sendMessage(msg);

    // TODO
    switch (e.type)
    {
        case BatteryEvent::Type::PING:
        break;
        
        case BatteryEvent::Type::BATTERY_FULL:
        break;

        case BatteryEvent::Type::BATTERY_LOW:
        break;

        case BatteryEvent::Type::BATTERY_DEAD:
        break;
    }
}
#endif

void PuppetMaster::gotPlayerEvent(const PlayerEvent &e)
{
    
    if (e.type == PlayerEvent::NewFrame)
    {
        if (player.numFailed > 0)
        {
            OSCMessage msg("/failed");
            msg.add(BOARD_NAME.c_str());
            msg.add(player.numFailed);
            osc.sendMessage(msg);
        }

        int dataIndex = 0;
        #ifdef NUM_LEDS
        for (int compIndex = 0; compIndex < NUM_LEDS; compIndex++)
        {
            if (e.data[dataIndex] < 255 && e.data[dataIndex + 1] < 255  && e.data[dataIndex + 2] < 255) // 255 value means don't update
                led.setColor(compIndex, (int)e.data[dataIndex], (int)e.data[dataIndex + 1], (int)e.data[dataIndex+2]);
            dataIndex += 3;
        }
        #endif
        #ifdef NUM_SERVOS
        for (int compIndex = 0; compIndex < NUM_SERVOS; compIndex++)
        {
            if (e.data[dataIndex] < 255) // 255 value means don't update
                servo.servoGoTo(compIndex, e.data[dataIndex] / 254.0f);
            dataIndex++;
        }
        #endif
        #ifdef NUM_STEPPERS
        for (int compIndex = 0; compIndex < NUM_STEPPERS; compIndex++)
        {
            // TODO A TESTER
            if (e.data[dataIndex] < 255) // 255 value means don't update
            motorwing.stepperSetSpeedRel(compIndex, e.data[dataIndex] / 127.0f - 1.0f);
            dataIndex++;
        }
        #endif
        #ifdef HAS_DCPORT
        // TODO A TESTER
        if (e.data[dataIndex] < 255) // 255 value means don't update
            motorwing.dcRun(MotorShield2Manager::DCPort::M1, e.data[dataIndex] / 127.0f - 1.0f);
        dataIndex++;
        if (e.data[dataIndex] < 255) // 255 value means don't update
            motorwing.dcRun(MotorShield2Manager::DCPort::M2, e.data[dataIndex] / 127.0f - 1.0f);
        dataIndex++;
        }
        #endif
    }
    
    if (e.type == PlayerEvent::Start)
    {
        compDebug("start playing");
    }
    
    if (e.type == PlayerEvent::Stop)
    {
        compDebug("stop playing");
    }
    
    if (e.type == PlayerEvent::Ended)
    {
        compDebug("ended");
    }
}


void PuppetMaster::gotFileEvent(const FileEvent &e)
{
    switch (e.type)
    {
        case FileEvent::Type::UploadStart:
            compDebug("File upload started");
            // TODO stop everything
            break;
            
        case FileEvent::Type::UploadProgress:
            compDebug("Uploading..."+String(e.value));
            break;
            
        case FileEvent::Type::UploadComplete:
            compDebug("Complete !");
            fileMgr.printFiles();
            advertiseSequences();
            break;
            
        case FileEvent::Type::Play:
            compDebug("play !");
            player.playSequence(e.fileName);
            break;
    }

}

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