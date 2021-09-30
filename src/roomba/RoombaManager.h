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

    bool handleCommand(OSCMessage &command) override;

protected:
    std::vector<RoombaSerial*> roombas; 

};
#endif