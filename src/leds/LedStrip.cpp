#include "LedStrip.h"

LedStrip::LedStrip(int pin, int numLeds, neoPixelType type, bool debug, bool useInSequences) : Component("led_" + String(pin)),
                                                                            numLeds(numLeds),
                                                                            strip(numLeds, pin, type),
                                                                            brightness(0.3),
                                                                            mode(LedMode::WAITING),
                                                                            toastTimer(0)
{
    // instantiate strip in init ?
    boolParameters["wifiDebug"] = debug;
    // TODO brightness parameter
    // toast duration parameter
    Component::useInSequences = useInSequences;
}

void LedStrip::initComponent(bool serialDebug)
{
    strip.begin();
    Component::initComponent(serialDebug);
}

void LedStrip::update()
{
    if (!checkInit())
        return;

    float slow = abs(sin(0.001f * millis()));
    float fast = abs(sin(0.002f * millis()));

    toastTimer.update();
    LedMode currentMode = toastTimer.isRunning ? toastMode : mode;

    // clear leds
    //if (!toastTimer.isRunning && toastMode != mode)
    //{
    //    compDebug("end toast");
    //    toastMode = mode;
    //    setColor(0, 0, 0);
    //}

    if (boolParameters["wifiDebug"])
        switch (currentMode)
        {
        case LedMode::WAITING:
            setAll(0, 0, int(50 * slow));
            break;

        case LedMode::READY:
            setAll(0, 100, 0);
            break;

        case LedMode::WORKING:
            setAll(int(50 * fast), 0, int(50 * fast));
            break;

        case LedMode::ERROR:
            setAll(50, 0, 0);
            break;

        case LedMode::COIN:
            setAll(int(250 * slow), int(250 * slow), int(250 * slow));
            break;

        case LedMode::STREAMING:
            break;
        }
    strip.show();
}

void LedStrip::clear()
{
    if (!checkInit())
        return;

    strip.clear();
    strip.show();
}

void LedStrip::setWifiDebug(bool value)
{
    boolParameters["wifiDebug"] = value;
    overrideFlashParameters();
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
        compError("incorrect rg value: " + String(g));
        return;
    }

    if (b < 0 || b > 255)
    {
        compError("incorrect  b value: " + String(b));
        return;
    }
    strip.setPixelColor(i, strip.Color((int)(r), (int)(g), (int)(b)));
    //strip.show();
}