#include "LedStrip.h"

LedStrip::LedStrip(int pin, int numLeds, neoPixelType type, bool debug, bool useInSequences) : Component("led_" + String(pin)),
                                                                            numLeds(numLeds),
                                                                            strip(numLeds, pin, type),
                                                                            brightness(0.3),
                                                                            notificationFade(0),
                                                                            isNotifying(false)
{
    // instantiate strip in init ?
    wifiDebug = debug;
    // boolParameters["wifiDebug"] = debug;
    // TODO brightness parameter
    // toast duration parameter
    Component::useInSequences = useInSequences;
}

void LedStrip::initComponent(bool serialDebug)
{
    strip.begin();
    Component::initComponent(serialDebug);
}

void LedStrip::notify(LedStrip::Notification notification)
{
    switch (notification)
    {
        case Notification::READY:
            notificationFade = 1.0f;
            break;
    }
    currentNotification = notification;
    isNotifying = true;
}

void LedStrip::update()
{
    if (!checkInit())
        return;

    float slow = abs(sin(0.001f * millis()));
    float fast = abs(sin(0.002f * millis()));
    
    if (isNotifying)
        switch (currentNotification)
        {
        case Notification::READY:
            setAll(100 * notificationFade, 100 * notificationFade, 100 * notificationFade);
            notificationFade *= 0.95f;
            break;

        case Notification::ERROR:
            setAll(50, 0, 0);
            break;

        case Notification::BOOTING:
            setAll(0, 0, int(50 * slow));
            break;
            
        case Notification::SHOW:
            setAll(250, 168, 60);
            break;

        case Notification::WORKING:
            setAll(int(50 * fast), int(50 * fast), int(50 * fast));
            break;

        case Notification::WAITING:
            setAll(int(50 * slow), 0, int(50 * slow));
            break;
            
    //         setAll(int(250 * slow), int(250 * slow), int(250 * slow));
        }
    strip.show();
}

void LedStrip::clear()
{
    if (!checkInit())
        return;

    isNotifying = false;
    strip.clear();
    strip.show();
}

void LedStrip::setWifiDebug(bool value)
{
    wifiDebug = value;
    //boolParameters["wifiDebug"] = value;
    //overrideFlashParameters();
}

bool LedStrip::isWifiDebug()
{
    return wifiDebug;
    //boolParameters["wifiDebug"];
}
/* TODO make childClass DebugLedStrip with communication methods
void LedStrip::setMode(LedMode newMode)
{
    //compDebug("set mode");
    //if (newMove != mode)
    //    setColor(0, 0, 0);
    //mode = newMode;
}

void LedStrip::toast(LedMode toastedMode, long ms)
{
    toastMode = toastedMode;

    toastTimer.time = ms;

    toastTimer.start();
}*/

void LedStrip::setBrightness(float value)
{
    if (value < 0.0f || value > 1.0f)
    {
        compError("incorrect value: " + String(value));
        return;
    }
    strip.setBrightness(255*value);
    compDebug("set brightness " + String(value));
}

void LedStrip::setAll(int c)
{
    setAll(c, c, c);
}

void LedStrip::setAll(int r, int g, int b)
{
    if (!checkInit())
        return;

    for (int i = 0; i < numLeds; i++)
        setLed(i, r, g, b);
}

void LedStrip::setLed(int i, int c)
{
    setLed(i, c, c, c);
}

void LedStrip::setLed(int i, int r, int g, int b)
{
    if (!checkInit())
        return;

    if (i < 0 || i >= numLeds)
    {
        compError("incorrect index: " + String(i));
        return;
    }

    if (r < 0 || r > 255)
    {
        compError("incorrect  r value: " + String(r));
        return;
    }

    if (g < 0 || g > 255)
    {
        compError("incorrect g value: " + String(g));
        return;
    }

    if (b < 0 || b > 255)
    {
        compError("incorrect  b value: " + String(b));
        return;
    }
    strip.setPixelColor(i, strip.Color((int)(r), (int)(g), (int)(b)));
}