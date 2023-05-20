#include "RoombaManager.h"

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

void RoombaManager::registerRoomba(int inPin, int outPin, int wakePin)
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
    roombas.back()->addListener(std::bind(&RoombaManager::gotRoombaEvent, this, std::placeholders::_1));

    compDebug("roomba device registered succesfully !");
}

void RoombaManager::wakeUp(int index)
{
    roombas[index]->wakeUp();
}

void RoombaManager::startMode(int index, RoombaMode mode)
{
    roombas[index]->start(mode);
}

void RoombaManager::getBattery(int index)
{
    roombas[index]->getBattery();
}
void RoombaManager::streamBattery(int index)
{
    roombas[index]->streamBattery();
}

void RoombaManager::setText(int index, String text)
{
    roombas[index]->setText(text);
}

void RoombaManager::drive(int index, float left, float right)
{
    //left = constrain(left, -1.0f, 1.0f);
    //right = constrain(right, -1.0f, 1.0f);
    roombas[index]->driveWheels(left, right);
}

void RoombaManager::setMaxSpeed(int index, float speed)
{
    roombas[index]->setMaxSpeed(speed);
}

void RoombaManager::setMotors(int index, bool vacuum, bool mainBrush, bool sideBrush)
{
    roombas[index]->setMotors(vacuum, mainBrush, sideBrush);
}

void RoombaManager::setHomeLed(int index, bool state)
{
    roombas[index]->setLed(HOME_GREEN, state);
}

void RoombaManager::setDirtLed(int index, bool state)
{
    roombas[index]->setLed(DIRT_BLUE, state);
}

void RoombaManager::setWarningLed(int index, bool state)
{
    roombas[index]->setLed(WARNING_RED, state);
}

void RoombaManager::setSpotLed(int index, bool state)
{
    roombas[index]->setLed(SPOT_GREEN, state);
}

void RoombaManager::setCenterHue(int index, int value)
{
    roombas[index]->setCenterHue(value);
}

void RoombaManager::setCenterBrightness(int index, int value)
{
    roombas[index]->setCenterBrightness(value);
}

void RoombaManager::playSong(int index, String name)
{
    if (name.equals("imperial")) roombas[index]->imperialSong();
    if (name.equals("victory")) roombas[index]->victorySong();
    if (name.equals("validate")) roombas[index]->validateSong();
    if (name.equals("cancel")) roombas[index]->cancelSong();
    if (name.equals("error")) roombas[index]->errorSong();
    if (name.equals("kraftwerk")) roombas[index]->kraftwerk();
}

void RoombaManager::playNote(int index, byte pitch, byte duration)
{
    roombas[index]->playNote(pitch, duration);
}

void RoombaManager::gotRoombaEvent(const RoombaValueEvent &e)
{
    sendEvent(e);
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
        if (checkCommandArguments(command, "ff", false))
        {
            drive(0, command.getFloat(0), command.getFloat(1));
            return true;
        } 
        else if (checkCommandArguments(command, "i", false))
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
            // TODO fix this
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
        // if (checkCommandArguments(command, "i", false))
        // {
        //     drive(command.getInt(0), 0, 0);
        //     return true;
        // } 
        drive(0, 0, 0);
        return true;
    }
    else if (address.equals("/roomba/speed"))
    {
        if (checkCommandArguments(command, "f", true))
        {
            setMaxSpeed(0, command.getFloat(0));
            return true;
        } 
        // if (checkCommandArguments(command, "if", false))
        // {
        //     setMaxSpeed(command.getInt(0), command.getFloat(1));
        //     return true;
        // } 
    }
    else if (address.equals("/roomba/brushes"))
    {
        if (checkCommandArguments(command, "bbb", false))
        {
            setMotors(0, command.getBoolean(0), command.getBoolean(1), command.getBoolean(2));
            return true;
        } 
        // if (checkCommandArguments(command, "ibbb", false))
        // {
        //     setMotors(command.getInt(0), command.getBoolean(1), command.getBoolean(2), command.getBoolean(3));
        //     return true;
        // } 
        else if (checkCommandArguments(command, "iii", true))
        {
            setMotors(0, command.getInt(0)>0, command.getInt(1)>0, command.getInt(2)>0);
            return true;
        } 
    }
    else if (address.equals("/roomba/vacuum"))
    {
        if (checkCommandArguments(command, "b", false))
        {
            setMotors(0, command.getBoolean(0), false, false);
            return true;
        } 
        // if (checkCommandArguments(command, "ii", false))
        // {
        //     setMotors(command.getInt(0), command.getInt(1)>0, false, false);
        //     return true;
        // } 
        // if (checkCommandArguments(command, "ib", false))
        // {
        //     setMotors(command.getInt(0), command.getBoolean(1), false, false);
        //     return true;
        // } 
        else if (checkCommandArguments(command, "i", true))
        {
            setMotors(0, command.getInt(0)>0, false, false);
            return true;
        } 
    }
    else if (address.equals("/roomba/main"))
    {
        if (checkCommandArguments(command, "b", false))
        {
            setMotors(0, false, command.getBoolean(0), false);
            return true;
        } 
        // else if (checkCommandArguments(command, "ii", false))
        // {
        //     setMotors(command.getInt(0), false, command.getInt(1)>0, false);
        //     return true;
        // } 
        // else if (checkCommandArguments(command, "ib", false))
        // {
        //     setMotors(command.getInt(0), false, command.getBoolean(1), false);
        //     return true;
        // } 
        else if (checkCommandArguments(command, "i", true))
        {
            setMotors(0, false, command.getInt(0)>0, false);
            return true;
        } 
    }
    else if (address.equals("/roomba/side"))
    {
        if (checkCommandArguments(command, "b", false))
        {
            setMotors(0, false, false, command.getBoolean(0));
            return true;
        } 
        // if (checkCommandArguments(command, "ii", false))
        // {
        //     setMotors(command.getInt(0), false, false, command.getInt(1)>0);
        //     return true;
        // } 
        // if (checkCommandArguments(command, "ib", false))
        // {
        //     setMotors(command.getInt(0), false, false, command.getBoolean(1));
        //     return true;
        // } 
        if (checkCommandArguments(command, "i", true))
        {
            setMotors(0, false, false, command.getInt(0)>0);
            return true;
        } 
    }
    else if (address.equals("/roomba/text"))
    {
        if (checkCommandArguments(command, "s", true))
        {
            char text[32];
            command.getString(0, text);
            setText(0, String(text));
            return true;
        } 
        // if (checkCommandArguments(command, "is", false))
        // {
        //     char text[32];
        //     command.getString(1, text);
        //     setText(command.getInt(0), String(text));
        //     return true;
        // } 
    }
    else if (address.equals("/roomba/song"))
    {
        if (checkCommandArguments(command, "s", true))
        {
            char song[32];
            command.getString(0, song);
            playSong(0, String(song));
            return true;
        }
    }
    else if (address.equals("/roomba/home"))
    {
        if (checkCommandArguments(command, "i", true))
        {
            setHomeLed(0, command.getInt(0)>0);
            return true;
        } 
    }
    else if (address.equals("/roomba/warning"))
    {
        if (checkCommandArguments(command, "i", true))
        {
            setWarningLed(0, command.getInt(0)>0);
            return true;
        } 
    }
    else if (address.equals("/roomba/dirt"))
    {
        if (checkCommandArguments(command, "i", true))
        {
            setDirtLed(0, command.getInt(0)>0);
            return true;
        } 
    }
    else if (address.equals("/roomba/hue"))
    {
        if (checkCommandArguments(command, "i", true))
        {
            setCenterHue(0, command.getInt(0));
            return true;
        } 
    }
    else if (address.equals("/roomba/brightness"))
    {
        if (checkCommandArguments(command, "i", true))
        {
            setCenterBrightness(0, command.getInt(0));
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
        wakeUp(0);
        return true;
    }
    else if (address.equals("/roomba/start"))
    {
        if (checkCommandArguments(command, "i", true))
        {
            int mode = command.getInt(0);
            if (mode >= 0 && mode < 3)
            {
                startMode(0, RoombaMode(mode));
                return true;
            }
        } 
    }
    else if (address.equals("/roomba/note"))
    {
        if (checkCommandArguments(command, "ii", true))
        {
            playNote(0, command.getInt(0), command.getInt(1));
            return true;
        }
    }
    else if (address.equals("/roomba/battery"))
    {
        getBattery(0);
        return true;
    }
    else if (address.equals("/roomba/stream"))
    {
        streamBattery(0);
        return true;
    }
    return false;
}