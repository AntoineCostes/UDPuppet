#pragma once
#include "../common/Manager.h"
#include "RoombaSerial.h"

class RoombaManager : public Manager
{
public:
    RoombaManager();

    void initManager();
    void update();
    
    void registerRoomba(int inPin, int outPin, int wakePin);

    void setText(int index, String text);

    void drive(int index, float left, float right);
    void setMaxSpeed(int index, float speed);

    void setHomeLed(int index, bool state);
    void setDirtLed(int index, bool state);
    void setWarningLed(int index, bool state);
    void setSpotLed(int index, bool state);
    void setCenterHue(int index, byte value);
    void setCenterBrightness(int index, byte value);

    bool handleCommand(OSCMessage &command) override;

protected:
    std::vector<RoombaSerial*> roombas; 

};