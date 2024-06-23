#include "MusicMakerManager.h"

MusicMakerManager::MusicMakerManager() : Manager("musicmaker"),
                                            ready(false)
{
    floatParameters["volume"] = 0.5f;
    serialDebug = MASTER_DEBUG;
}

void MusicMakerManager::initManager()
{
    Manager::initManager();
    
    std::set<int> pins = {VS1053_CS, VS1053_DCS, VS1053_DREQ, CARDCS, MISO, MOSI, SCK};
    if (!Component::registerPins(pins))
    {
        compError("cannot register Music Maker: a pin is already registered !");
        return;
    }

    player = new Adafruit_VS1053_FilePlayer(VS1053_RESET, VS1053_CS, VS1053_DCS, VS1053_DREQ, CARDCS);
    if (!player->begin())
    {
        compError("Couldn't find VS1053!");
        return;
    }
    if (!SD.begin(CARDCS))
    {
        compError("Couldn't mount SD!");
        return;
    }
#ifndef ESP32
    player->useInterrupt(VS1053_DREQ); // fait crasher l'ESP32
#endif

    ready = true;

    tracks.clear();
    File root =  SD.open("/");
    File file = root.openNextFile();
    if (file) compDebug("Audio tracks list:");
    String fName;
    while(file)
    {
        fName = file.name();
        if (fName.endsWith(".mp3") || fName.endsWith(".wav"))
        {
            tracks.emplace_back(fName);
            compDebug(fName);
        }
        file =  root.openNextFile();
    }
    setVolume(floatParameters["volume"]);
}

void MusicMakerManager::update()
{
#ifdef ESP32
    if (player->playingMusic) player->feedBuffer(); // a la place d'un interrupt
#endif
}

bool MusicMakerManager::handleCommand(OSCMessage &command)
{
    if (!ready)
    {
        compError("music maker not ready");
        return false;
    }

    char buf[32];
    command.getAddress(buf);
    String address = String(buf);
    compLog("handle command : " + address);

    if (address.equals("/musicmaker/play"))
    {
        if (checkCommandArguments(command, "s", true))
        {
            char trackName[32];
            command.getString(0, trackName);
            play(String(trackName));
            return true;
        }
    }
    if (address.equals("/musicmaker/stop"))
    {
        stop();
        return true;
    }
    if (address.equals("/musicmaker/pause"))
    {
        togglePausePlay();
        return true;
    }
    if (address.equals("/musicmaker/volume"))
    {
        if (checkCommandArguments(command, "f", true))
        {
            setVolume(command.getFloat(0));
            return true;
        }
    }
    return false;
}

void MusicMakerManager::setVolume(float value)
{
    if (!ready)
    {
        compError("music maker not ready");
        return;
    }
    if (value < 0.0f or value > 1.0f)
    {
        compError("wrong volume range");
        return;
    }
    int volume = 100*(1.0f-value);
    compDebug("set volume: "+String(volume));
    player->setVolume(volume, volume);
    
    floatParameters["volume"] = value;
    overrideFlashParameters();
}

float MusicMakerManager::getVolume()
{
    return floatParameters["volume"];
}

bool MusicMakerManager::isReady()
{
    return ready;
}

bool MusicMakerManager::play(String trackName)
{
    if (!ready)
    {
        compError("music maker not ready");
        return false;
    }
    
    stop();
    
    compDebug("play " + trackName);

    if (std::find(tracks.begin(), tracks.end(), trackName) == tracks.end()) 
    {
        compError(trackName+" file does not exist!");
        return false;
    }
    trackName = "/"+trackName;

    compDebug("start playing");
    player->startPlayingFile(trackName.c_str());
    return true;
}

void MusicMakerManager::stop()
{ 
    compDebug("stop call");
    if (!ready)
    {
        compError("music maker not ready");
        return;
    }
    if (player->playingMusic)
    {
        compDebug("stop playing");
        player->stopPlaying();
    }
}

void MusicMakerManager::togglePausePlay()
{ 
    if (!ready)
    {
        compError("music maker not ready");
        return;
    }
    player->pausePlaying(!player->paused());
}