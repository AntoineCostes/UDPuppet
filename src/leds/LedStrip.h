
#pragma once
#ifdef NUM_STRIPS
#include "../common/Component.h"
#include "../utils/Timer.h"

class LedStrip : public Component
{
public:
    LedStrip(int pin, int numLeds, neoPixelType type, bool debug, bool useInSequences);
    //~LedStrip(){Serial.println("delete LedStrip");}
    //~LedStrip() {}

    void initComponent(bool serialDebug);
    void update();
    
    // allows to display a given mode for a given duration
    //void toast(LedMode toastedMode, long ms);
    //void setMode(LedMode mode);

    void clear();
    void setWifiDebug(bool value);
    void setBrightness(float value);
    void setAll(int c);
    void setAll(int r, int g, int b);
    void setLed(int i, int c);
    void setLed(int i, int r, int g, int b);

    enum LedMode
    {
        WAITING,
        READY,
        WORKING,
        ERROR,
        COIN,
        STREAMING
    } mode;

protected:
    int numLeds;
    float brightness;
    Adafruit_NeoPixel strip;
    Timer toastTimer;
    LedMode toastMode;
};
#endif