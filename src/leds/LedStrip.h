
#pragma once
#include <Adafruit_NeoPixel.h>
#include "../common/Component.h"

class LedStrip : public Component
{
public:
    LedStrip(int pin, int numLeds, neoPixelType type);
    //~LedStrip(){Serial.println("delete LedStrip");}
    //~LedStrip() {}

    void initComponent(bool serialDebug);
    void update();
    
    void clear();
    void setBrightness(float value);
    void setAll(int c);
    void setAll(int r, int g, int b);
    void setLed(int i, int c);
    void setLed(int i, int r, int g, int b);

protected:
    int numLeds;
    float brightness;
    Adafruit_NeoPixel strip;
};
