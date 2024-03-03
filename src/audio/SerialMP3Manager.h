#pragma once
#include "../common/Manager.h"


class SerialMP3Manager : public Manager
{
public:
    SerialMP3Manager();

    void initManager() override;
    void update() override;
    bool handleCommand(OSCMessage &command) override;

    void setVolume(float value);
    void play(int trackIndex);
    void playNext();
    void playConfirmSound();
    void playCancelSound();
    void stop();

    float getVolume();
    bool isPlaying();
    int getNumTracks();
    int getNextTrackIndex();

protected:
    bool ready;
    bool playing;
    int lastPlayedIndex;
    int numTracks;
    HardwareSerial MySerial0;
    MD_YX5300 mp3; 
};