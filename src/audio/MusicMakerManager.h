#pragma once
#ifdef HAS_MUSICMAKER
#include "../common/Manager.h"

#define VS1053_RESET   -1     // VS1053 reset pin (not used!)

#if defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S2)
#define VS1053_CS       6     // VS1053 chip select pin (output)
#define VS1053_DCS     10     // VS1053 Data/command select pin (output)
#define CARDCS          5     // Card chip select pin
// DREQ should be an Int pin *if possible* (not possible on 32u4)
#define VS1053_DREQ     9     // VS1053 Data request, ideally an Interrupt pin

#elif defined(ESP32)
#define VS1053_CS      32     // VS1053 chip select pin (output)
#define VS1053_DCS     33     // VS1053 Data/command select pin (output)
#define CARDCS         14     // Card chip select pin
#define VS1053_DREQ    15     // VS1053 Data request, ideally an Interrupt pin

#elif defined(ESP8266)
#define VS1053_CS      16     // VS1053 chip select pin (output)
#define VS1053_DCS     15     // VS1053 Data/command select pin (output)
#define CARDCS          2     // Card chip select pin
#define VS1053_DREQ     0     // VS1053 Data request, ideally an Interrupt pin

#endif

class MusicMakerManager : public Manager
{
public:
  MusicMakerManager();

  void initManager() override;
  void update() override;
  bool handleCommand(OSCMessage &command) override;

  std::vector<String> tracks;
  void setVolume(float value);
  void play(String trackName);
  void stop();
  void togglePausePlay();

  float getVolume();
  bool isReady();

protected:
  Adafruit_VS1053_FilePlayer* player;
  bool ready;

};
#endif