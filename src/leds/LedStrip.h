
#pragma once
#include "../common/Component.h"
#include "../utils/Timer.h"

class LedStrip : public Component
{
public:
    LedStrip(int pin, int numLeds, neoPixelType type, bool debug, bool useInSequences);
    //~LedStrip(){Serial.println("delete LedStrip");}
    //~LedStrip() {}

    void initComponent(bool serialDebug);
    void update() override;

    void clear();
    void setWifiDebug(bool value);
    void setBrightness(float value);
    void setAll(int c);
    void setAll(int r, int g, int b);
    void setLed(int i, int c);
    void setLed(int i, int r, int g, int b);

    bool isWifiDebug();
    
    enum Notification
    {
        READY,
        ERROR,
        BOOTING,
        CONNECTED,
        WORKING,
        WAITING,
        SHOW
    } currentNotification;
    void notify(LedStrip::Notification notification);
    bool isNotifying;

protected:
    int numLeds;
    float brightness;
    bool wifiDebug;
    float notificationFade;
    Adafruit_NeoPixel strip;
};