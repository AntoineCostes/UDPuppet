#pragma once
#include "../common/Manager.h"
#include "FileManager.h"

#define PLAYER_REFRESH 30

class PlayerEvent
{
public:
    enum Type
    {
        Start,
        NewFrame,
        Stop,
        Ended
    };

    PlayerEvent(Type t, String sequenceName, uint8_t * data = nullptr) : type(t), sequenceName(sequenceName), data(data) {}

    Type type;
    uint8_t * data;
    String sequenceName;
};

class SequencePlayer : public Manager,
      public EventBroadcaster<PlayerEvent>
{
public:
  SequencePlayer();

  void initManager(int frameSize);
  void update() override;

  void setFPS(int value);

  void playSequence(String path);
  bool isPlaying;
  void stopPlaying();
  void tryPlaySequence(String path);
  int numFailed;
  
  bool handleCommand(OSCMessage &command) override;

protected:
  uint8_t frameSize;
  uint8_t* frameData;

  void loadSequence(String path);
  void startSequence(float atTime);
  void playFrame();
  void seek(float t);

  File curFile;
  String curSequenceName;
  long lastPlayerMs;

  int fps;
  float totalTime;
  long totalFrames;

  //playing
  long curTimeMs;
  long prevTimeMs;
  long timeSinceLastSeek;
  float timeToSeek; //used to limit seeking
  bool doLoop;

  // TODO simplify this
  long msToFrame(long timeMs) { return timeMs * fps / 1000; }
  long msTointPos(long t) { return msToFrame(t) * frameSize; } //rgba
  long frameToMs(long frame) { return frame * 1000 / fps; }
  float frameToSeconds(long frame) { return frame * 1.0f / fps; };
  long secondsToMs(float s) { return s * 1000; }
  long intPosToFrame(long pos) { return pos / frameSize; }
  long intPosToMs(long pos) { return frameToMs(intPosToFrame(pos)); }
  long intPosToSeconds(long pos) { return frameToSeconds(intPosToFrame(pos)); }

  uint8_t *buff;
  int nbFrames = 0;

}; 