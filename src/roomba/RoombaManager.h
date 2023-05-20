#pragma once
#include "../common/Manager.h"
#include "RoombaSerial.h"
#include "../utils/EventBroadcaster.h"


// TODO remove index ?
// TODO play MIDI note

class RoombaManager : public Manager,
                    public EventBroadcaster<RoombaValueEvent>
{
public:
    RoombaManager();

    void initManager();
    void update();
    
    void registerRoomba(int inPin, int outPin, int wakePin);

    void wakeUp(int index);
    void startMode(int index, RoombaMode mode);
    void getBattery(int index);
    void streamBattery(int index);

    void drive(int index, float left, float right);
    void setMaxSpeed(int index, float speed);
    void setMotors(int index, bool vacuum, bool mainBrush, bool sideBrush);

    void setHomeLed(int index, bool state);
    void setDirtLed(int index, bool state);
    void setWarningLed(int index, bool state);
    void setSpotLed(int index, bool state);
    void setCenterHue(int index, int value);
    void setCenterBrightness(int index, int value);

    void setText(int index, String text);
    void playSong(int index, String name);
    void playNote(int index, byte pitch, byte duration);

    void gotRoombaEvent(const RoombaValueEvent &e);

    bool handleCommand(OSCMessage &command) override;

protected:
    std::vector<RoombaSerial*> roombas; 

};