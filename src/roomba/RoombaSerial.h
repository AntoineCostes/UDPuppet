#pragma once
#include "../common/Component.h"

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
    void victorySong();
    void validateSong();
    void cancelSong();
    void errorSong();
    void kraftwerk();

protected:
    byte wakePin;
    SoftwareSerial serial;

    bool ledStates[4];
    byte centerLedHue;
    byte centerLedBrightness;

    float maxSpeed;

    String textBuffer;
    long lastTextUpdate;

    enum Pitch
    {
        SILENCE = 30,
        G_1 = 31,
        GSharp_1 = 32,
        A_1 = 33,
        ASharp_1 = 34,
        B_1 = 35,
        C_2 = 36,
        CSharp_2 = 37,
        D_2 = 38,
        DSharp_2 = 39,
        E_2 = 40,
        F_2 = 41,
        FSharp_2 = 42,
        G_2 = 43,
        GSharp_2 = 44,
        A_2 = 45,
        ASharp_2 = 46,
        B_2 = 47,
        C_3 = 48,
        CSharp_3 = 49,
        D_3 = 50,
        DSharp_3 = 51,
        E_3 = 52,
        F_3 = 53,
        FSharp_3 = 54,
        G_3 = 55,
        GSharp_3 = 56,
        A_3 = 57,
        ASharp_3 = 58,
        B_3 = 59,
        C_4 = 60,
        CSharp_4 = 61,
        D_4 = 62,
        DSharp_4 = 63,
        E_4 = 64,
        F_4 = 65,
        FSharp_4 = 66,
        G_4 = 67,
        GSharp_4 = 68,
        A_4 = 69,
        ASharp_4 = 70,
        B_4 = 71,
        C_5 = 72,
        CSharp_5 = 73,
        D_5 = 74,
        DSharp_5 = 75,
        E_5 = 76,
        F_5 = 77,
        FSharp_5 = 78,
        G_5 = 79,
        GSharp_5 = 80,
        A_5 = 81,
        ASharp_5 = 82,
        B_5 = 83,
        C_6 = 84,
        CSharp_6 = 85,
        D_6 = 86,
        DSharp_6 = 87,
        E_6 = 88,
        F_6 = 89,
        FSharp_6 = 90,
        G_6 = 91,
        GSharp_6 = 92,
        A_6 = 93,
        ASharp_6 = 94,
        B_6 = 95,
        C_7 = 96,
        CSharp_7 = 97,
        D_7 = 98,
        DSharp_7 = 99,
        E_7 = 100,
        F_7 = 101,
        FSharp_7 = 102,
        G_7 = 103,
        GSharp_7 = 104,
        A_7 = 105,
        ASharp_7 = 106,
        C_8 = 108,
        CSharp_8 = 109,
        D_8 = 110,
        DSharp_8 = 111,
        E_8 = 112,
        F_8 = 113,
        FSharp_8 = 114,
        G_8 = 115,
        GSharp_8 = 116,
        A_8 = 117,
        ASharp_8 = 118,
        B_8 = 119,
        C_9 = 120,
        CSharp_9 = 121,
        D_9 = 122,
        DSharp_9 = 123,
        E_9 = 124,
        F_9 = 125,
        FSharp_9 = 126,
        G_9 = 127
    };
};