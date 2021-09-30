#include "RoombaManager.h"

#ifdef HAS_ROOMBA
RoombaManager::RoombaManager() : Manager("roomba")
{
    serialDebug = ROOMBA_DEBUG;
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

void RoombaManager::setText(byte index, String text)
{
    roombas[index]->setText(text);
}

void RoombaManager::drive(byte index, float left, float right)
{
    left = constrain(left, -1.0f, 1.0f);
    right = constrain(right, -1.0f, 1.0f);
    roombas[index]->driveWheels(left*500, right*500);
}

bool RoombaManager::handleCommand(OSCMessage &command)
{
    if (!checkInit())
        return false;

    char buf[32];
    command.getAddress(buf);
    String address = String(buf);
    compLog("handle command : " + address);

    if (address.equals("/roomba/move"))
    {
        if (checkCommandArguments(command, "i", false))
        {
            int direction = command.getInt(0);
            switch(direction)
            {
                case 0:
                    compDebug("forward");
                    roombas[0]->driveWheels(500,500);
                    break;
                    
                case 1:
                    compDebug("left");
                    roombas[0]->driveWheels(500,-500);
                    break;

                case 2:
                    compDebug("right");
                    roombas[0]->driveWheels(-500,500);
                    break;

                case 3:
                    compDebug("back");
                    roombas[0]->driveWheels(-500,-500);
                    break;
            }
            return true;
        } 
        else if (checkCommandArguments(command, "ii", true))
        {
            int direction = command.getInt(0);
            switch(command.getInt(1))
            {
                case 0:
                    compDebug("forward");
                    roombas[command.getInt(0)]->driveWheels(500,500);
                    break;
                    
                case 1:
                    compDebug("left");
                    roombas[command.getInt(0)]->driveWheels(500,-500);
                    break;

                case 2:
                    compDebug("right");
                    roombas[command.getInt(0)]->driveWheels(-500,500);
                    break;

                case 3:
                    compDebug("back");
                    roombas[command.getInt(0)]->driveWheels(-500,-500);
                    break;
            }
            return true;
        } 
    }
    else if (address.equals("/roomba/stop"))
    {
        if (checkCommandArguments(command, "i", false))
        {
            if(command.getInt(0))
            {
                roombas[0]->driveWheels(0,0);
            }
            return true;
        } 
        else if (checkCommandArguments(command, "ii", false))
        {
            if(command.getInt(1))
            {
                roombas[command.getInt(0)]->driveWheels(0,0);
            }
            return true;
        } 
    }
    else if (address.equals("/roomba/speed"))
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
    else if (address.equals("/roomba/text"))
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
    else if (address.equals("/roomba/song1"))
    {
        if (checkCommandArguments(command, "i", false))
        {   
            if(command.getInt(0))
                roombas[0]->imperialSong();
            return true;
        } 
    }
    else if (address.equals("/roomba/home"))
    {
        if (checkCommandArguments(command, "i", false))
        {
            roombas[0]->setLed(HOME_GREEN,command.getInt(0));
            return true;
        } 
    }
    else if (address.equals("/roomba/warning"))
    {
        if (checkCommandArguments(command, "i", false))
        {
            roombas[0]->setLed(WARNING_RED,command.getInt(0));
            return true;
        } 
    }
    else if (address.equals("/roomba/dirt"))
    {
        if (checkCommandArguments(command, "i", false))
        {
            roombas[0]->setLed(DIRT_BLUE,command.getInt(0));
            return true;
        } 
    }
    else if (address.equals("/roomba/hello"))
    {
        if (checkCommandArguments(command, "i", false))
        {
            roombas[0]->setText("Bonjour le monde !");
            return true;
        } 
    }
    return false;
}
#endif