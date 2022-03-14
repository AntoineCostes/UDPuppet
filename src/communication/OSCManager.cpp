#include "OSCManager.h"

OSCManager::OSCManager(WifiManager *wifiMgr, String mDNSName) : Manager("osc"),
                                                                wifi(wifiMgr),
                                                                overrideTargetIp(TARGET_IP_OVERRIDE),
                                                                mDNSName(mDNSName)
{
    stringParameters["targetIp"] = OSC_TARGET_IP;
    serialDebug = OSC_DEBUG;
}

void OSCManager::initManager()
{
    if (OSC_TARGET_IP.length() != 0) overrideFlashParameters(); // override if defined in config
    
    Manager::initManager();
    // subscribe to wifi notifications to handle UDP port
    wifi->addListener(std::bind(&OSCManager::gotWifiEvent, this, std::placeholders::_1));
}

// open UDP port and create mDNS service when network connection is successfull
void OSCManager::gotWifiEvent(const WifiEvent &e)
{
    switch (e.state)
    {
    case WifiConnectionState::CONNECTED:
        compDebug("wifi connected, try to connect");
        connect();
        break;

    case WifiConnectionState::DISCONNECTED:
        compDebug("wifi lost !");
        disconnect();
        break;

    default:
        break;
    }
}

void OSCManager::connect()
{
    if (!checkInit())
        return;

    compLog("Listening on " + String(OSC_LISTENING_PORT));
    compLog("Targeting " + stringParameters["targetIp"] + "@" + String(OSC_TARGET_PORT));

    compLog("Local IP: " + wifi->getIP());

    udp.begin(OSC_LISTENING_PORT);
    udp.flush();
    lastSentPingMs = millis();

    compLog("creating mDNS instance: " + mDNSName);
    if (MDNS.begin(mDNSName.c_str()))
    {
        MDNS.addService("_osc", "_udp", OSC_LISTENING_PORT);
        compDebug("OSC Zeroconf service added sucessfully !");
    }
    else
    {
        sendEvent(OSCEvent(OSCEvent::Type::MDNS_ERROR));
        compError("could not set up mDNS instance");
    }
    isConnected = true;
    sendEvent(OSCEvent(OSCEvent::Type::CONNECTED));
}

// TODO: is that really clean ?
void OSCManager::disconnect()
{
    if (!isConnected)
        return;

    udp.flush();
    udp.stop();
    MDNS.end();
    isConnected = false;
}

void OSCManager::update()
{
    if (!isConnected)
        return;

    if (millis() > lastSentPingMs + PING_TIMEOUT_MS)
    {
        OSCMessage pingMsg("/ping");
        pingMsg.add(BOARD_NAME.c_str());
        sendMessage(pingMsg);
        lastSentPingMs = millis();
    }

    OSCMessage msg;
    int size;
    if ((size = udp.parsePacket()) > 0)
    {
        while (size--)
            msg.fill(udp.read());
        if (!msg.hasError())
        {
            compDebug("got osc message");
                
            // when receiving messages from new IP, makes this the new target
            if (stringParameters["targetIp"] != udp.remoteIP().toString())
            {
                stringParameters["targetIp"] = udp.remoteIP().toString();
                if (overrideTargetIp)
                    overrideFlashParameters();
                compLog("new target: " + String(OSC_TARGET_PORT) + "@" + stringParameters["targetIp"]);
            }

            if (msg.match("/yo"))
            {
                sendEvent(OSCEvent(OSCEvent::Type::HANDSHAKE));
            }
            else if (msg.match("/ping"))
            {
                sendEvent(OSCEvent(OSCEvent::Type::PING_ALIVE));
            }
            else
            {
            sendEvent(OSCEvent(&msg));
            } 
            // create command from OSCMessage
            // char addr[32];
            // msg.getAddress(addr, 1); //remove the first slash
            // String tc(addr);
            // int tcIndex = tc.indexOf('/');

            // int numData = msg.size();
            // var *msgData = (var *)malloc(numData * sizeof(var));
            // int numUsedData = 0;

            // char tmpStr[32][32]; //contains potential strings

            // for (int i = 0; i < msg.size(); i++)
            // {
            //     switch (msg.getType(i))
            //     {
            //     case 'i':
            //         msgData[i].value.i = msg.getInt(i);
            //         msgData[i].type = 'i';
            //         numUsedData++;
            //         break;
            //     case 'f':
            //         msgData[i].value.f = msg.getFloat(i);
            //         msgData[i].type = 'f';
            //         numUsedData++;
            //         break;
            //     case 's':
            //         msg.getString(i, tmpStr[i]);
            //         msgData[i].value.s = tmpStr[i];
            //         msgData[i].type = 's';
            //         numUsedData++;
            //         break;

            //     default:
            //         break;
            //     }
            // }
            //handle_command(tc.substring(0, tcIndex), tc.substring(tcIndex + 1), msgData, numUsedData);
        }
    }
    return;
}

void OSCManager::pong()
{
    OSCMessage pongMsg("/pong");
    pongMsg.add(BOARD_NAME.c_str());
    pongMsg.add(wifi->getMAC().c_str());
    sendMessage(pongMsg);
}

void OSCManager::yo()
{
    OSCMessage answerMsg("/yo");
    answerMsg.add(BOARD_NAME.c_str());
    answerMsg.add(wifi->getIP().c_str());
    answerMsg.add(wifi->getMAC().c_str());
    sendMessage(answerMsg);
}

void OSCManager::sendMessage(OSCMessage &msg)
{
    if (!checkInit())
        return;

    if (!isConnected)
    {
        compError("Can't send message, OSC not connected !");

        char addr[32];
        msg.getAddress(addr);
        compError(addr);
        return;
    }

    char addr[32];
    msg.getAddress(addr);
    compDebug("Send message to " + stringParameters["targetIp"] + "@" + String(OSC_TARGET_PORT) + " : " + String(addr));
    udp.beginPacket((char *)stringParameters["targetIp"].c_str(), (uint16_t)OSC_TARGET_PORT);
    msg.send(udp);
    udp.endPacket();
    msg.empty();
}

void OSCManager::sendMessage(String address)
{
    OSCMessage m(address.c_str());
    sendMessage(m);
}