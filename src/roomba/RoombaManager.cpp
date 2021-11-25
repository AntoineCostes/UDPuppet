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

    std::set<byte> pins = {inPin, outPin, wakePin};
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

void RoombaManager::setMaxSpeed(byte index, float speed)
{
    roombas[index]->setMaxSpeed(speed);
}

void RoombaManager::setHomeLed(byte index, bool state)
{
    roombas[index]->setLed(HOME_GREEN, state);
}

void RoombaManager::setDirtLed(byte index, bool state)
{
    roombas[index]->setLed(DIRT_BLUE, state);
}

void RoombaManager::setWarningLed(byte index, bool state)
{
    roombas[index]->setLed(WARNING_RED, state);
}

void RoombaManager::setSpotLed(byte index, bool state)
{
    roombas[index]->setLed(SPOT_GREEN, state);
}

void RoombaManager::setCenterHue(byte index, byte value)
{
    roombas[index]->setCenterHue(value);
}

void RoombaManager::setCenterBrightness(byte index, byte value)
{
    roombas[index]->setCenterBrightness(value);
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
                    drive(0, 1.0f, 1.0f);
                    break;
                    
                case 1:
                    compDebug("left");
                    drive(0, 1.0f, -1.0f);
                    break;

                case 2:
                    compDebug("right");
                    drive(0, -1.0f, 1.0f);
                    break;

                case 3:
                    compDebug("back");
                    drive(0, -1.0f, -1.0f);
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
                    drive(command.getInt(0), 1.0f, 1.0f);
                    break;
                    
                case 1:
                    compDebug("left");
                    drive(command.getInt(0), 1.0f, -1.0f);
                    break;

                case 2:
                    compDebug("right");
                    drive(command.getInt(0), -1.0f, 1.0f);
                    break;

                case 3:
                    compDebug("back");
                    drive(command.getInt(0), -1.0f, -1.0f);
                    break;
            }
            return true;
        } 
    }
    else if (address.equals("/roomba/stop"))
    {
        if (checkCommandArguments(command, "i", false))
        {
            drive(command.getInt(0), 0, 0);
            return true;
        } 
        drive(0, 0, 0);
        return true;
    }
    else if (address.equals("/roomba/speed"))
    {
        if (checkCommandArguments(command, "f", false))
        {
            setMaxSpeed(0, command.getFloat(0));
            return true;
        } 
        else if (checkCommandArguments(command, "if", false))
        {
            setMaxSpeed(command.getInt(0), command.getFloat(1));
            return true;
        } 
    }
    else if (address.equals("/roomba/text"))
    {
        if (checkCommandArguments(command, "s", false))
        {
            char text[32];
            command.getString(0, text);
            setText(0, String(text));
            return true;
        } 
        else if (checkCommandArguments(command, "is", false))
        {
            char text[32];
            command.getString(1, text);
            setText(command.getInt(0), String(text));
            return true;
        } 
    }
    else if (address.equals("/roomba/song/imperial"))
    {
        roombas[0]->imperialSong();
        return true;
    }
    else if (address.equals("/roomba/song/victory"))
    {
        roombas[0]->victorySong();
        return true;
    }
    else if (address.equals("/roomba/song/validate"))
    {
        roombas[0]->validateSong();
        return true;
    }
    else if (address.equals("/roomba/song/cancel"))
    {
        roombas[0]->cancelSong();
        return true;
    }
    else if (address.equals("/roomba/song/error"))
    {
        roombas[0]->errorSong();
        return true;
    }
    else if (address.equals("/roomba/song/kraftwerk"))
    {
        roombas[0]->kraftwerk();
        return true;
    }
    else if (address.equals("/roomba/home"))
    {
        if (checkCommandArguments(command, "i", false))
        {
            setHomeLed(0, command.getInt(0)>0);
            return true;
        } 
    }
    else if (address.equals("/roomba/warning"))
    {
        if (checkCommandArguments(command, "i", false))
        {
            setWarningLed(0, command.getInt(0)>0);
            return true;
        } 
    }
    else if (address.equals("/roomba/dirt"))
    {
        if (checkCommandArguments(command, "i", false))
        {
            setDirtLed(0, command.getInt(0)>0);
            return true;
        } 
    }
    else if (address.equals("/roomba/hue"))
    {
        if (checkCommandArguments(command, "i", false))
        {
            setCenterHue(0, (byte)command.getInt(0));
            return true;
        } 
    }
    else if (address.equals("/roomba/brightness"))
    {
        if (checkCommandArguments(command, "i", false))
        {
            setCenterBrightness(0, (byte)command.getInt(0));
            return true;
        } 
    }
    else if (address.equals("/roomba/hello"))
    {
        setText(0, "Bonjour le monde !");
        return true;
    }
    else if (address.equals("/roomba/wake"))
    {
        roombas[0]->wakeUp();
        return true;
    }
    else if (address.equals("/roomba/start"))
    {
        roombas[0]->startSafe();
        return true;
    }
    return false;
}
#endif