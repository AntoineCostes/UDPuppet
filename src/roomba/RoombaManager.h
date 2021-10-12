#pragma once
#include "../common/Manager.h"
#include "RoombaSerial.h"

#ifdef HAS_ROOMBA
class RoombaManager : public Manager
{
public:
    RoombaManager();

    void initManager();
    void update();
    
    void registerRoomba(byte inPin, byte outPin, byte wakePin);

    void setText(byte index, String text);

    void drive(byte index, float left, float right);
    void setMaxSpeed(byte index, float speed);

    void setHomeLed(byte index, bool state);
    void setDirtLed(byte index, bool state);
    void setWarningLed(byte index, bool state);
    void setSpotLed(byte index, bool state);
    void setCenterHue(byte index, byte value);
    void setCenterBrightness(byte index, byte value);

    bool handleCommand(OSCMessage &command) override;

protected:
    std::vector<RoombaSerial*> roombas; 

};
#endif