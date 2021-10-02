#pragma once
#include "../common/Component.h"

#ifdef HAS_ROOMBA
#include <SoftwareSerial.h>

enum RoombaLed
{
    DIRT_BLUE,
    SPOT_GREEN,
    HOME_GREEN,
    WARNING_RED
};

class RoombaSerial : public Component
{
public:
    // inPin = roomba RX = brown wire
    // outPin = roomba TX = black wire
    // wakePin = green wire
    RoombaSerial(byte inPin, byte outPin, byte wakePin);
    //~RoombaSerial(){Serial.println("delete RoombaSerial");}
    //~RoombaSerial() {}

    void initComponent(bool serialDebug);
    void update();
    
    // general methods
    void wakeUp();
    void startSafe();

    // methods for leds
    void setLed(RoombaLed led, bool state);
    void setCenterHue(byte value);
    void setCenterBrightness(byte value);
    void updateLeds();
    
    // methods for 7 segment display
    void setText(String text);
    void appendText(String text);
    void setDigitLEDs(byte digit1, byte digit2, byte digit3, byte digit4);
    void setDigitLEDFromASCII(byte digit, char letter);
    void writeLEDs (char a, char b, char c, char d);

    // methods for motors
    void setMaxSpeed(float value);
    void drive(int velocity, int radius);
    void driveWheels(int right, int left);
    void driveWheelsPWM(int rightPWM, int leftPWM);

    // methods for sounds
    void imperialSong();

protected:
    byte wakePin;
    SoftwareSerial serial;

    bool ledStates[4];
    byte centerLedHue;
    byte centerLedBrightness;

    float maxSpeed;

    String textBuffer;
    long lastTextUpdate;
};
#endif