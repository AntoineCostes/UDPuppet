#pragma once
#include "../common/Manager.h"

#ifdef HAS_MUSICMAKER
#include <SPI.h>
#include <SD.h>
#include <Adafruit_VS1053.h>

#define VS1053_RESET   -1     // VS1053 reset pin (not used!)
#define VS1053_CS      32     // VS1053 chip select pin (output)
#define VS1053_DCS     33     // VS1053 Data/command select pin (output)
#define CARDCS         14     // Card chip select pin
#define VS1053_DREQ    15     // VS1053 Data request, ideally an Interrupt pin

class MusicMakerManager : public Manager
{
public:
  MusicMakerManager();

  void initManager() override;
  bool handleCommand(OSCMessage &command) override;

  std::vector<String> tracks;
  void setVolume(float value);
  void play(String trackName);
  void stop();
  void togglePausePlay();

  float getVolume();

protected:
  Adafruit_VS1053_FilePlayer* player;
  bool ready;

};
#endif