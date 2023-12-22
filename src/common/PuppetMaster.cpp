#include "PuppetMaster.h"

// fix library 
// AsyncWebSocket.cpp l832 
// return IPAddress((uint32_t) 0U);

// CLEAN
// passe sur les TODO et les FIXME
// separer stepper et DC
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

// TODO
// refactor managers log functions (use dbg, log, err)
// OSC error feedback
// used pins feedback => give names to reservedPins ?
// make battery monitoring a pref option (ESP32)
// change debug from Chataigne
// checkComponents
// readFlashParameters for 8266
// implement microSeconds for servo
// port in handshake ?
// JSON config files ?
// component bool checkRange(float/int min, float/int max)

// remove initComponent
// checkInit only in Manager ?
// initComponent (when registering) and updating components automatically

// ERROR mode if prop was not registered successfully
// make childClass DebugLedStrip with communication methods

PuppetMaster::PuppetMaster() : Manager("master"),
                               osc(&wifi),
                               firmwareVersion("1.4.11")
{
#ifdef BASE 
    // Base uses pin 12 and 13
    // don't register any pins

#elif defined(HAS_ROOMBA) 
    // Roomba has led on pin 12
    Component::registerPin(LED_BUILTIN)
    ; 
#elif defined(HAS_MUSICMAKER) && defined(ESP8266)
    // musicmaker uses huzzah8662 pin #0
    
#else
    Component::registerPin(LED_BUILTIN); 

#if (BOARD_TYPE == HUZZAH32)
    Component::registerPin(12); // This pin has a pull-down resistor built into it, we recommend using it as an output only, or making sure that the pull-down is not affected during boot.
#endif

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
    if (WIFI_CREDENTIALS.ssid != "")
    {
        managers.emplace_back(&wifi);
        wifi.initManager();
    }

    managers.emplace_back(&osc);
    osc.initManager();
    wifi.addListener(std::bind(&PuppetMaster::gotWifiEvent, this, std::placeholders::_1));
    osc.addListener(std::bind(&PuppetMaster::gotOSCEvent, this, std::placeholders::_1));

#ifdef ESP32
    managers.emplace_back(&battery);
    battery.initManager();
    battery.addListener(std::bind(&PuppetMaster::gotBatteryEvent, this, std::placeholders::_1));
#endif

#ifdef HAS_MUSICMAKER
    managers.emplace_back(&musicmaker);
    musicmaker.initManager();
    fileMgr.init(musicmaker.isReady());
#else
    fileMgr.init();
#endif

    managers.emplace_back(&player);
    player.initManager();
    player.addListener(std::bind(&PuppetMaster::gotPlayerEvent, this, std::placeholders::_1));

    if (FileManager::doesExist("/index.html"))
    {
        managers.emplace_back(&web);
        web.initManager();
        web.addListener(std::bind(&PuppetMaster::gotFileEvent, this, std::placeholders::_1));
    } else
    {
        compError("webserver not initiliazed: index.hmtl was not found");
    }

    managers.emplace_back(&button);
    button.initManager();
    button.addListener(std::bind(&PuppetMaster::gotButtonEvent, this, std::placeholders::_1));

#ifdef NUM_STRIPS
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
    motorwing.addListener(std::bind(&PuppetMaster::gotMotorwingStepperEvent, this, std::placeholders::_1));
#endif

#ifdef HAS_STEPPER_DRIVER
    managers.emplace_back(&stepperdriver);
    stepperdriver.initManager();
    stepperdriver.addListener(std::bind(&PuppetMaster::gotStepperEvent, this, std::placeholders::_1));
#endif

#ifdef HAS_ROOMBA
    managers.emplace_back(&roomba);
    roomba.initManager();
    roomba.addListener(std::bind(&PuppetMaster::gotRoombaValueEvent, this, std::placeholders::_1));
#endif


    // TODO give this info on demand
    // compDebug("forbidden pins: ");
    // for (int pin : Component::forbiddenPins)
    //     compDebug(String(pin));
    
}

void PuppetMaster::advertiseComponents()
{
    compDebug("advertise components");

#ifdef HAS_ADALOGGER_WING
if (true) // hack to delete OSCMessage object - TODO send(addr, args)
{
    OSCMessage msg("/adalogger/sd");
    msg.add(BOARD_NAME.c_str());
    msg.add(fileMgr.sdIsDetected?(int32_t)1:(int32_t)0);
    osc.sendMessage(msg);
}
#endif

#ifdef HAS_MUSICMAKER
if (true)
{
    OSCMessage msg("/musicmaker/tracks");
    msg.add(BOARD_NAME.c_str());
    for (auto seq : musicmaker.tracks) msg.add(seq.c_str());
    osc.sendMessage(msg);
}
if (true)
{
    OSCMessage msg("/musicmaker/volume");
    msg.add(BOARD_NAME.c_str());
    msg.add(musicmaker.getVolume());
    osc.sendMessage(msg);
}
if (true)
{
    OSCMessage msg("/musicmaker/sd");
    msg.add(BOARD_NAME.c_str());
    msg.add(musicmaker.isReady()?(int32_t)1:(int32_t)0);
    osc.sendMessage(msg);
}
#endif

    advertiseSequences();

// FIXME /dc/0
#ifdef HAS_MOTORWING
    for (auto &pair : motorwing.dcMotors)
    {
        String addr = "/dc/maxspeed/"+String(int(pair.first));
        OSCMessage msg(addr.c_str());
        msg.add(BOARD_NAME.c_str());
        msg.add((int32_t)pair.second->getMaxSpeed());
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
    switch (BOARD_TYPE)
    {
    case HUZZAH32:
    case HUZZAH8266:
    case THINGESP8266:
    case HUZZAH32_S3:
    case XIAO_C3:
        break;

        break;

    default:
        compError("---------------");
        compError("Can't run: Board type not supported!");
        while(1);
        return;
    }

}

void PuppetMaster::advertiseSequences()
{
    OSCMessage msg("/files/sequences");
    msg.add(BOARD_NAME.c_str());
    for (auto seq : fileMgr.sequences) msg.add(seq.c_str());
    osc.sendMessage(msg);
}

void PuppetMaster::update()
{
    for (auto const &mgr : managers)
    {
        if (mgr.get()->checkInit())
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


    if (command.match("/coin"))
    {
#ifdef NUM_STRIPS
        led.setMode(LedStrip::LedMode::COIN);
#endif
    }

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
        fileMgr.deleteFileIfExists("/"+String(str)+".dat");
        advertiseSequences();
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
    musicmaker.play(sequenceName+".mp3");
    #endif 
}

void PuppetMaster::gotWifiEvent(const WifiEvent &e)
{
    switch (e.state)
    {
    case WifiEvent::ConnectionState::CONNECTING:
        wifi.dbg("connecting to wifi...");
        digitalWrite(LED_BUILTIN, HIGH);
    #ifdef NUM_STRIPS
        led.setMode(LedStrip::LedMode::WORKING);
    #endif
        break;

    case WifiEvent::ConnectionState::CONNECTED:
        wifi.dbg("wifi connected !");
        digitalWrite(LED_BUILTIN, LOW);
        

        wifi.log("creating mDNS instance: " + BOARD_NAME);// BOARD_NAME+ " v" + "1.3.5"));
        if (MDNS.begin(BOARD_NAME.c_str()))
        {
            MDNS.addService("_osc", "_udp", OSC_LISTENING_PORT);
            MDNS.addService("_http", "_tcp", 80);
             wifi.dbg("OSC Zeroconf service added sucessfully !");
        }
        else
        {
            wifi.err("could not set up mDNS instance");
        }

        // FIXME wifiDebug
    #ifdef NUM_STRIPS
        led.setMode(LedStrip::LedMode::READY);
        //led.setColor(0, 0, 50, 0);
        // led.toast(LedStrip::LedMode::READY, 1000); // probleme: ca reste vert si pas de stream
    #endif
    #ifdef NUM_SERVOS
        for (int i = 0; i < NUM_SERVOS; i++) servo.servoGoTo(i, 0.0f);
        delay(500);
        for (int i = 0; i < NUM_SERVOS; i++) servo.servoGoTo(i, 0.6f);
        delay(500);
        for (int i = 0; i < NUM_SERVOS; i++) servo.servoGoToStart(i);
    #endif

        web.initServer();
        
        break;

    case WifiEvent::ConnectionState::DISCONNECTED:
        wifi.dbg("wifi lost !");
        digitalWrite(LED_BUILTIN, HIGH);
    #ifdef NUM_STRIPS
        led.setMode(LedStrip::LedMode::ERROR);
    #endif
        break;

    default:
        wifi.err("wifi state not handled");
        break;
    }
}

void PuppetMaster::gotOSCEvent(const OSCEvent &e)
{
    switch (e.type)
    {
    case OSCEvent::Type::CONNECTED:
        // TODO advertise error
        osc.yo(firmwareVersion);
        advertiseComponents();
        break;

    case OSCEvent::Type::DEAD_PING:
        // TODO advertise error
        break;

    case OSCEvent::Type::PING_ALIVE: // not used
        //osc.pong();
        break;

    case OSCEvent::Type::HANDSHAKE:
        osc.yo(firmwareVersion);
        advertiseComponents();
        break;

    case OSCEvent::Type::COMMAND:
        sendCommand(*e.command);
        break;

    default:
        compLog("OSCEvent :" + String(e.type));
        break;
    }
}

void PuppetMaster::gotButtonEvent(const ButtonEvent &e)
{
    switch (e.type)
    {
#ifdef BUTTON_JUKEBOX
    case ButtonEvent::Type::PRESSED:
        musicmaker.stop();
        player.stopPlaying();
        
#ifdef NUM_SERVOS
        for (int i = 0; i < NUM_SERVOS; i++) servo.servoGoTo(i, 0.0f);
#endif

#ifdef NUM_STRIPS
    led.clear();
#endif
        break;

    case ButtonEvent::Type::RELASED_SHORT:
        // launchSequence(fileMgr.sequences[trackIndex]);
        launchSequence(REPERTOIRE[trackIndex]);
        trackIndex++;
        // if (trackIndex >= fileMgr.sequences.size()) trackIndex = 0;
        if (trackIndex >= REPERTOIRE_LENGTH) trackIndex = 0;
        compLog("track index :" + String(trackIndex));
        break;

    case ButtonEvent::Type::LONG_PRESS:
        compLog("long press");
        musicmaker.play("cancel.mp3");
        break;
#endif

    default:
        button.log("ButtonEvent :" + String(e.type));
        break;
    }
}

#ifdef HAS_MOTORWING
void PuppetMaster::gotMotorwingStepperEvent(const StepperEvent &e)
{
    if (!osc.isConnected)
        return;

    //compDebug("stepper event");

    OSCMessage msg("/stepper/pos"); //+String(e.index)));
    msg.add(BOARD_NAME.c_str());
    msg.add((int32_t)e.position);
    msg.add((int32_t)e.speed);
    msg.add((float)e.maxSpeed);
    osc.sendMessage(msg);
}
#endif

#ifdef HAS_ROOMBA
void PuppetMaster::gotRoombaValueEvent(const RoombaValueEvent &e)
{
    if (!osc.isConnected)
        return;

    OSCMessage msg("/roomba/battery"); 
    msg.add((int32_t)e.rawValue);
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
    msg.add((int32_t)e.position);
    msg.add((int32_t)e.speed);
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
    msg.add((int32_t) e.level);
    msg.add((int32_t) e.voltage);
    msg.add((int32_t) e.analogValue);
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
            msg.add((int32_t)player.numFailed);
            osc.sendMessage(msg);
        }

        int dataIndex = 0;
        #ifdef NUM_STRIPS
        for (int compIndex = 0; compIndex < NUM_STRIPS; compIndex++)
        {
            if (led.useInSequences(compIndex))
            {
                if (e.data[dataIndex] < 255 && e.data[dataIndex + 1] < 255  && e.data[dataIndex + 2] < 255) // 255 value means don't update
                    led.setColor(compIndex, (int)e.data[dataIndex], (int)e.data[dataIndex + 1], (int)e.data[dataIndex+2]);
                dataIndex += 3;
            }
        }
        #endif
        #ifdef NUM_SERVOS
        for (int compIndex = 0; compIndex < NUM_SERVOS; compIndex++)
        {
            if (servo.useInSequences(compIndex))
            {
                if (e.data[dataIndex] < 255) // 255 value means don't update
                    servo.servoGoTo(compIndex, e.data[dataIndex] / 254.0f);
                dataIndex++;
            }
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
        // player.dbg("start playing");
    }
    
    if (e.type == PlayerEvent::Stop)
    {
        player.dbg("stop playing");
    }
    
    if (e.type == PlayerEvent::Ended)
    {
        player.dbg("ended");

// #ifdef BUTTON_JUKEBOX
// #ifdef NUM_STRIPS
// led.setMode(LedStrip::LedMode::WAITING);
// #endif
// #endif

#ifdef CONTINUE_PLAYING
        launchSequence(fileMgr.sequences[trackIndex]);
        trackIndex++;
        if (trackIndex >= fileMgr.sequences.size()) trackIndex = 0;
        compLog("track index :" + trackIndex);
#endif
    }
}


void PuppetMaster::gotFileEvent(const FileEvent &e)
{
    switch (e.type)
    {
        case FileEvent::Type::UploadStart:
            fileMgr.dbg("File upload started");
            // TODO stop everything
            break;
            
        case FileEvent::Type::UploadProgress:
            fileMgr.dbg("Uploading..."+String(e.value));
            break;
            
        case FileEvent::Type::UploadComplete:
            fileMgr.dbg("Complete !");
            fileMgr.printFiles();
            advertiseSequences();
            break;
            
        case FileEvent::Type::Play:
            fileMgr.dbg("play !");
            player.playSequence(e.fileName);
            break;
    }

}