#include "../common/Manager.h"
#include "FileManager.h"

#ifdef HAS_SD_WING

#define PLAYER_REFRESH 20

#ifdef AMPOULE
#define FRAME_SIZE 4 // 1 pixel (RGB) + 1 servo
#elif defined BASE
#define FRAME_SIZE 3 // 2 servo + 1 stepper
#elif defined BOBINE
#define FRAME_SIZE 1 // 1 stepper
#elif defined CORBEILLE
#define FRAME_SIZE 2 // 2 DC
#elif defined TEST
#define FRAME_SIZE 6 // 
#endif


class PlayerEvent
{
public:
    enum Type
    {
        Start,
        NewFrame,
        Stop
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

  void setFPS(byte value);

  void loadSequence(String path);

  void startSequence(float atTime);

void stopPlaying();
  
  bool handleCommand(OSCMessage &command) override;

protected:

  uint8_t frameData[FRAME_SIZE];

  void playFrame();
  void seek(float t);

  File curFile;
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
  long msToBytePos(long t) { return msToFrame(t) * FRAME_SIZE; } //rgba
  long frameToMs(long frame) { return frame * 1000 / fps; }
  float frameToSeconds(long frame) { return frame * 1.0f / fps; };
  long secondsToMs(float s) { return s * 1000; }
  long bytePosToFrame(long pos) { return pos / FRAME_SIZE; }
  long bytePosToMs(long pos) { return frameToMs(bytePosToFrame(pos)); }
  long bytePosToSeconds(long pos) { return frameToSeconds(bytePosToFrame(pos)); }

  uint8_t *buff;
  int nbFrames = 0;

}; 
#endif // HAS_SD_WING