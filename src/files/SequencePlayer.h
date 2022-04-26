#pragma once
#include "../common/Manager.h"
#include "FileManager.h"

#define PLAYER_REFRESH 30

#ifdef AMPOULE
#define FRAME_SIZE 4 // 1 pixel (RGB) + 1 servo
#elif defined BASE
#define FRAME_SIZE 3 // 2 servo + 1 stepper
#elif defined BOBINE
#define FRAME_SIZE 1 // 1 stepper
#elif defined CORBEILLE
#define FRAME_SIZE 2 // 2 DC
#elif defined CAMEMBERT
#define FRAME_SIZE 1 // 1 servo 
#elif defined CHANTDRIER
#define FRAME_SIZE 4 // 4 servos 
#else
#define FRAME_SIZE 4 // TODO remove and compute automatically
#endif


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

    PlayerEvent(Type t, uint8_t * data = nullptr) : type(t), data(data) {}

    Type type;
    uint8_t * data;
};

class SequencePlayer : public Manager,
      public EventBroadcaster<PlayerEvent>
{
public:
  SequencePlayer();

  void initManager();
  void update() override;

  void setFPS(int value);

  void loadSequence(String path);

  void startSequence(float atTime);

  void tryPlaySequence(String path);
  int numFailed;

  void playSequence(String path);

  void stopPlaying();
  
  bool handleCommand(OSCMessage &command) override;

protected:
  uint8_t frameData[FRAME_SIZE];

  void playFrame();
  void seek(float t);

  File curFile;
  String curSequenceName;
  long lastPlayerMs;

  int fps;
  float totalTime;
  long totalFrames;

  //playing
  bool isPlaying;
  long curTimeMs;
  long prevTimeMs;
  long timeSinceLastSeek;
  float timeToSeek; //used to limit seeking
  bool doLoop;

  // TODO simplify this
  long msToFrame(long timeMs) { return timeMs * fps / 1000; }
  long msTointPos(long t) { return msToFrame(t) * FRAME_SIZE; } //rgba
  long frameToMs(long frame) { return frame * 1000 / fps; }
  float frameToSeconds(long frame) { return frame * 1.0f / fps; };
  long secondsToMs(float s) { return s * 1000; }
  long intPosToFrame(long pos) { return pos / FRAME_SIZE; }
  long intPosToMs(long pos) { return frameToMs(intPosToFrame(pos)); }
  long intPosToSeconds(long pos) { return frameToSeconds(intPosToFrame(pos)); }

  uint8_t *buff;
  int nbFrames = 0;

}; 