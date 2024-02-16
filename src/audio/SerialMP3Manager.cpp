#include "SerialMP3Manager.h"

SerialMP3Manager::SerialMP3Manager() : Manager("serialmp3"),
                                            ready(false),
                                            numTracks(0),
                                            MySerial0(0),
                                            mp3(MySerial0)
{
    floatParameters["volume"] = 0.5f;
    serialDebug = MASTER_DEBUG;
}

void SerialMP3Manager::initManager()
{
    Manager::initManager();
    
    MySerial0.begin(MD_YX5300::SERIAL_BPS, SERIAL_8N1, -1, -1); // default RX TY pins
    mp3.begin();

    delay(50);
    update();
    mp3.queryFilesCount();
    delay(50);
    update();
    setVolume(floatParameters["volume"]);
}

void SerialMP3Manager::update()
{
    if (mp3.check())
    {
        const MD_YX5300::cbData *status = mp3.getStatus();
        switch (status->code)
        {
            case MD_YX5300::STS_TOT_FILES:  
                numTracks = status->data;
                compDebug("number of tracks: "+String(numTracks));  
                break;
                
            case MD_YX5300::STS_ERR_FILE:   
                compDebug("STS_ERR_FILE");   
                break;

            case MD_YX5300::STS_FILE_END:   
                compDebug("STS_FILE_END");   
                break;
                
            case MD_YX5300::STS_TF_INSERT:  compDebug("STS_TF_INSERT");  break;
            case MD_YX5300::STS_TF_REMOVE:  compDebug("STS_TF_REMOVE");  break;

            case MD_YX5300::STS_OK:         compDebug("STS_OK");         break;
            case MD_YX5300::STS_INIT:       compDebug("STS_INIT");       break;
            case MD_YX5300::STS_TIMEOUT:    compDebug("STS_TIMEOUT");    break;
            case MD_YX5300::STS_VERSION:    compDebug("STS_VERSION");    break;
            case MD_YX5300::STS_CHECKSUM:   compDebug("STS_CHECKSUM");   break;
            case MD_YX5300::STS_ACK_OK:     compDebug("STS_ACK_OK");     break;
            case MD_YX5300::STS_STATUS:     compDebug("STS_STATUS");     break;
            case MD_YX5300::STS_EQUALIZER:  compDebug("STS_EQUALIZER");  break;
            case MD_YX5300::STS_VOLUME:     compDebug("STS_VOLUME");     break;
            case MD_YX5300::STS_PLAYING:    compDebug("STS_PLAYING");    break;
            case MD_YX5300::STS_FLDR_FILES: compDebug("STS_FLDR_FILES"); break;
            case MD_YX5300::STS_TOT_FLDR:   compDebug("STS_TOT_FLDR");   break;
            default: compDebug("STS_???"); break;
        }
        compDebug(String(status->data));
    }
}

bool SerialMP3Manager::handleCommand(OSCMessage &command)
{
    char buf[32];
    command.getAddress(buf);
    String address = String(buf);
    compLog("handle command : " + address);

    if (address.equals("/serialmp3/play"))
    {
        if (checkCommandArguments(command, "i", true))
        {
            play(command.getInt(0));
            return true;
        }
    }
    if (address.equals("/serialmp3/stop"))
    {
        stop();
        return true;
    }
    if (address.equals("/serialmp3/volume"))
    {
        if (checkCommandArguments(command, "f", true))
        {
            setVolume(command.getFloat(0));
            return true;
        }
    }
    if (address.equals("/serialmp3/query"))
    {
        mp3.queryStatus();
        return true;
    }
    return false;
}

void SerialMP3Manager::setVolume(float value)
{
    if (value < 0.0f or value > 1.0f)
    {
        compError("wrong volume range");
        return;
    }
    compDebug("set volume "+String(value*30));
    mp3.volume(value*30); 
    
    floatParameters["volume"] = value;
    overrideFlashParameters();
    mp3.queryVolume();
}

float SerialMP3Manager::getVolume()
{
    return floatParameters["volume"];
}

void SerialMP3Manager::play(int trackIndex)
{
    compDebug("play track "+String(trackIndex));
    mp3.playTrack(trackIndex+1);
}

void SerialMP3Manager::stop()
{ 
    compDebug("stop playing");
    mp3.playStop();
}
