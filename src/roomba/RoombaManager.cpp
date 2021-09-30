#include "RoombaManager.h"

#ifdef HAS_ROOMBA
RoombaManager::RoombaManager() : Manager("roomba")
{
    //boolParameters["debug"] = true;
}

void RoombaManager::initManager()
{
    Manager::initManager();
}

void RoombaManager::update()
{
    if (!checkInit())
        return;

    for (auto const &roomba : roombas)
    {
        roomba->update();
    }
}

void RoombaManager::registerRoomba(byte inPin, byte outPin, byte wakePin)
{
    if (!checkInit())
        return;

    std::set<int> pins = {inPin, outPin, wakePin};
    if (!Component::registerPins(pins))
    {
        compError("cannot register roomba: a pin is already registered !");
        return;
    }

    roombas.emplace_back(new RoombaSerial(inPin, outPin, wakePin));
    roombas.back()->initComponent(serialDebug);
    compDebug("roomba registered.");
}


bool RoombaManager::handleCommand(OSCMessage &command)
{
    if (!checkInit())
        return false;

    char buf[32];
    command.getAddress(buf);
    String address = String(buf);
    compLog("handle command : " + address);

    if (address.equals("/move"))
    {
        if (checkCommandArguments(command, "i", false))
        {
            switch(command.getInt(0))
            {
                case 0:
                    compDebug("zero");
                    break;
                    
                case 1:
                    compDebug("one");
                    break;

                case 2:
                    compDebug("two");
                    break;

                case 3:
                    compDebug("three");
                    break;
            }
            return true;
        } 
        else if (checkCommandArguments(command, "ii", true))
        {
            switch(command.getInt(1))
            {
                case 0:
                    compDebug("zero i");
                    break;
                    
                case 1:
                    compDebug("one i");
                    break;

                case 2:
                    compDebug("two i");
                    break;

                case 3:
                    compDebug("three i");
                    break;
            }
            return true;
        } 
    }
    else if (address.equals("/stop"))
    {
        if (checkCommandArguments(command, "i", false))
        {
            if(command.getInt(0))
                compDebug("stop");
            return true;
        } 
        else if (checkCommandArguments(command, "ii", false))
        {
            if(command.getInt(1))
                compDebug("stop index");
            return true;
        } 
    }
    else if (address.equals("/speed"))
    {
        if (checkCommandArguments(command, "f", false))
        {
            compDebug(String(command.getFloat(0)));
            return true;
        } 
        else if (checkCommandArguments(command, "if", false))
        {
            compDebug(String(command.getFloat(1)));
            return true;
        } 
    }
    else if (address.equals("/text"))
    {
        if (checkCommandArguments(command, "s", false))
        {
            char text[32];
            command.getString(0, text);
            roombas[0]->setText(String(text));
            return true;
        } 
        else if (checkCommandArguments(command, "is", false))
        {
            char text[32];
            command.getString(1, text);
            roombas[command.getInt(0)]->setText(String(text));
            return true;
        } 
    }
    return false;
}
#endif