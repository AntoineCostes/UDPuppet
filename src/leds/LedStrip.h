
#pragma once
#include "../common/Component.h"
#include "../utils/Timer.h"


#ifdef HAS_LED
#include <Adafruit_NeoPixel.h>
class LedStrip : public Component
{
public:
    LedStrip(byte pin, int numLeds, neoPixelType type, bool debug);
    //~LedStrip(){Serial.println("delete LedStrip");}
    //~LedStrip() {}

    void initComponent(bool serialDebug) override;
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