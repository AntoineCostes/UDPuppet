#pragma once
#include "../common/Manager.h"
#include "../utils/EventBroadcaster.h"

#include <FS.h>
#include <SD.h>

#ifdef ESP32
    #include <SPIFFS.h>
    #define SD_CS 33

#elif defined(ESP8266)
    #define SD_CS 15
#endif

#ifdef HAS_ADALOGGER_WING
    #define SD_MISO 19 // Adalogger FeatherWing
    #define SD_MOSI 18 // Adalogger FeatherWing
    #define SD_SCK 5 // Adalogger FeatherWing

#elif defined(HAS_MUSICMAKER)
    #define SD_MISO 12 // Music Maker FeatherWing
    #define SD_MOSI 11 // Music Maker FeatherWing
    #define SD_SCK 13 // Music Maker FeatherWing
#endif

// use SD instead of SPIFFS
#ifndef ESP32
    #ifdef HAS_MUSICMAKER
        #define USE_SD
    #endif
#endif
#if defined(HAS_ADALOGGER_WING)
    #define USE_SD
#endif

class FileManager : public Manager//, public EventBroadcaster<FileEvent>//, public EventBroadcaster<SDEvent>
{
public:
    FileManager();
    ~FileManager() {}

#ifdef USE_SD
// use SD instead of SPIFFS
    static SPIClass spiSD;
    static bool sdIsDetected;
#endif

    void init();
    void update() override;
    
    //File manipulation
    static boolean doesExist(String fileName);
    static File openFile(String fileName, bool forWriting = false, bool deleteIfExists = true);
    static void deleteFileIfExists(String path);
    void printFiles();

    std::vector<String> sequences;

    // bool handleCommand(String command, var *data, int numData) override;

    protected:
    void listDir(const char *dirname, uint8_t levels);

};

