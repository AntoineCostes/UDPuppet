#include "LedStrip.h"

#ifdef HAS_LED
LedStrip::LedStrip(int pin, int numLeds, neoPixelType type) : Component("led_pin" + String(pin)),
                                                              numLeds(numLeds),
                                                              strip(numLeds, pin, type)
{
    // pin as intParameter ? instantiate strip in init ?
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

    strip.show();
}

void LedStrip::clear()
{
    if (!checkInit())
        return;

    strip.clear();
    strip.show();
}

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
#endif