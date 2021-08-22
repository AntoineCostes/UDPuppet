#pragma once
#include "../common/Manager.h"
#include "../utils/Timer.h"
#include "LedStrip.h"

class LedManager : public Manager
{
public:
    LedManager();

    void initManager();
    void update();
    
    void registerLedStrip(byte index, byte pin, byte numLeds, neoPixelType type);
    
    enum LedMode
    {
        WAITING,
        READY,
        WORKING,
        ERROR,
        STREAMING
    } mode;

    // allows to display a given mode for a given duration
    void toast(LedMode toastedMode, long ms);
    void setMode(LedMode mode);
    
    // all
    void clear();
    void setColor(int c);
    void setColor(int r, int g, int b);
    void setBrightness(float value);
    
    // strip
    void setColor(int stripIndex, int c);
    void setColor(int stripIndex, int r, int g, int b);
    void setBrightness(int stripIndex, float value);

    // led
    void setColor(int stripIndex, int i, int r, int g, int b);
    
    void setDebug(bool value);
    bool handleCommand(OSCMessage &command) override;

protected:
    unsigned long lastLedRefreshTime;
    std::map<int, LedStrip*> strips; 

    Timer toastTimer;
    LedMode toastMode;
};
