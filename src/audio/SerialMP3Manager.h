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
    void stop();

    float getVolume();

    uint8_t numTracks;

protected:
    bool ready;
    HardwareSerial MySerial0;
    MD_YX5300 mp3; 

};