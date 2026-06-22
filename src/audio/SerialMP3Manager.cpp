#include "SerialMP3Manager.h"

SerialMP3Manager::SerialMP3Manager() : Manager("serialmp3"),
                                            ready(false),
                                            playing(false),
                                            numTracks(0),
                                            lastPlayedIndex(0),
                                            MySerial0(0),
                                            mp3(MySerial0),
                                            lastStopMs(0)
{
    floatParameters["volume"] = 0.5f;
    serialDebug = SERIALMP3_DEBUG;
}

// the library doesn't seem to handle folders
// => the two last sounds on the SD card are used for confirm and cancel notification

void SerialMP3Manager::initManager()
{
    Manager::initManager();
    
    MySerial0.begin(MD_YX5300::SERIAL_BPS, SERIAL_8N1, -1, -1); // default RX TY pins
    mp3.begin();
    mp3.setSynchronous(false);              // returns STS_ACK_OK
    mp3.queryFilesCount();                  // STS_TOT_FILES
    delay(50);
    update();
    setVolume(floatParameters["volume"]);   // returns STS_ACK_OK
    delay(50);
    update();
    // those delays + update were found empirically to ensure proper init
}

void SerialMP3Manager::update()
{
    // hack for speaker standby :  after 5mn silence play a sound 
    if (!isPlaying() && millis() - lastStopMs > 3000000)
    {
        playConfirmSound();
    }

    if (mp3.check())
    {
        const MD_YX5300::cbData *status = mp3.getStatus();
        switch (status->code)
        {
            case MD_YX5300::STS_ACK_OK:     
                compDebug("STS_ACK_OK " + String(status->data)); 
                if (!ready)  ready = true; 
                break;

            case MD_YX5300::STS_TOT_FILES:  
                numTracks = status->data - 2; // the two last tracks are cancel and confirm sounds
                compDebug("number of tracks: "+String(numTracks));  
                playConfirmSound();
                lastPlayedIndex = numTracks; 
                break;
                
            case MD_YX5300::STS_ERR_FILE:   
                compDebug("STS_ERR_FILE " + String(status->data));  
                if (playing)
                {
                    // compDebug("failed, try again");
                    play(lastPlayedIndex);
                    playing = false;
                }
                break;

            case MD_YX5300::STS_FILE_END:   
                compDebug("STS_FILE_END " + String(status->data));  
                playing = false;
                lastStopMs = millis();
                break;
                
            case MD_YX5300::STS_TF_INSERT:  
                compDebug("STS_TF_INSERT "  + String(status->data)); 
                mp3.reset();
                delay(500);
                update();
                initManager();
                break;

            case MD_YX5300::STS_TF_REMOVE:  
                compDebug("STS_TF_REMOVE "  + String(status->data));
                ready = false; 
                numTracks = 0;
                lastPlayedIndex = 0;
                break;

            case MD_YX5300::STS_OK:         compDebug("STS_OK "         + String(status->data)); break;
            case MD_YX5300::STS_INIT:       compDebug("STS_INIT "       + String(status->data)); break;
            case MD_YX5300::STS_TIMEOUT:    compDebug("STS_TIMEOUT "    + String(status->data)); break;
            case MD_YX5300::STS_VERSION:    compDebug("STS_VERSION "    + String(status->data)); break;
            case MD_YX5300::STS_CHECKSUM:   compDebug("STS_CHECKSUM "   + String(status->data)); break;
            case MD_YX5300::STS_STATUS:     compDebug("STS_STATUS "     + String(status->data)); break;
            case MD_YX5300::STS_EQUALIZER:  compDebug("STS_EQUALIZER "  + String(status->data)); break;
            case MD_YX5300::STS_VOLUME:     compDebug("STS_VOLUME "     + String(status->data)); break;
            case MD_YX5300::STS_PLAYING:    compDebug("STS_PLAYING "    + String(status->data)); break;
            case MD_YX5300::STS_FLDR_FILES: compDebug("STS_FLDR_FILES " + String(status->data)); break;
            case MD_YX5300::STS_TOT_FLDR:   compDebug("STS_TOT_FLDR "   + String(status->data)); break;
            default: compDebug("STS_???"); break;
        }
    }
}

void SerialMP3Manager::play(int trackIndex)
{
    compDebug("play track "+String(trackIndex));
    if (!ready)
    {
        compError("not ready yet, querying files...");
        mp3.queryFilesCount();
        return;
    }
    if (trackIndex < 0 || trackIndex > numTracks - 1)
    {
        compError("track index = "+ String(trackIndex) + " should be within [0-" + String(numTracks - 1) +"]");
        return;
    }
    mp3.playTrack((uint8_t) (trackIndex + 1) ); // the index starts at 1

    lastPlayedIndex = trackIndex;
    playing = true;
    // compDebug("is playing now !");
}

void SerialMP3Manager::stop()
{ 
    if (isPlaying())
    {
    compDebug("stop playing");
    mp3.playStop();
    playing = false;
    lastStopMs = millis();
    } else compDebug("stop playing (wasn't)");
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
    // mp3.queryVolume(); doesn't work ?
}

float SerialMP3Manager::getVolume()
{
    return floatParameters["volume"];
}

int SerialMP3Manager::getNumTracks()
{
    return numTracks;
}

int SerialMP3Manager::getNextTrackIndex()
{
    // compDebug("get next, current = " + String(lastPlayedIndex) + " / " + String(numTracks));
    if (lastPlayedIndex < numTracks - 1) return lastPlayedIndex + 1;
    else return 0;
}

bool SerialMP3Manager::isPlaying()
{
    // compDebug("is playing ? " + String(playing));
    return playing;
}

void SerialMP3Manager::playNext()
{
    play(getNextTrackIndex());
}

void SerialMP3Manager::playConfirmSound()
{
    compLog("play confirm");
    if (!ready)
    {
        compError("not ready yet, querying files...");
        mp3.queryFilesCount();
        return;
    }
    mp3.playTrack((uint8_t)(numTracks + 2)); 
    playing = true;
}

void SerialMP3Manager::playCancelSound()
{
    compLog("play cancel");
    if (!ready)
    {
        compError("not ready yet, querying files...");
        mp3.queryFilesCount();
        return;
    }
    mp3.playTrack((uint8_t)(numTracks + 1)); 
    playing = true;
}