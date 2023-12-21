#pragma once
#ifdef NUM_STRIPS
#include "../common/Manager.h"
#include "LedStrip.h"

class LedManager : public Manager
{
public:
    LedManager();

    void initManager() override;
    void update() override;
    
    void registerLedStrip(int pin, int numLeds, neoPixelType type, bool wifiDebug, bool useInSequences);

    void setMode(LedStrip::LedMode newMode);
    
    // all
    void clear();
    void setColor(int c);
    void setColor(int r, int g, int b);
    void setBrightness(float value);
    
    // strip
    void setColor(int stripIndex, int c);
    void setColor(int stripIndex, int r, int g, int b);
    void setBrightness(int stripIndex, float value);
    void setWifiDebug(int stripIndex, bool value);
    bool useInSequences(int stripIndex);

    // led
    void setColor(int stripIndex, int i, int r, int g, int b);
    
    bool handleCommand(OSCMessage &command) override;

protected:
    unsigned long lastRefreshTime;
    std::vector<LedStrip*> strips; 
};
#endif