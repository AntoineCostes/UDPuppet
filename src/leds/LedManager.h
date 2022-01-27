#pragma once
#include "../common/Manager.h"
#include "LedStrip.h"

const long LED_REFRESH_MS = 20;

#ifdef NUM_LEDS
class LedManager : public Manager
{
public:
    LedManager();

    void initManager(bool serialDebug);
    void update();
    
    void registerLedStrip(int index, int pin, int numLeds, neoPixelType type, boolean wifiDebugAll);

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

    // led
    void setColor(int stripIndex, int i, int r, int g, int b);
    
    bool handleCommand(OSCMessage &command) override;

protected:
    unsigned long lastLedRefreshTime;
    std::map<int, LedStrip*> strips; 
};
#endif